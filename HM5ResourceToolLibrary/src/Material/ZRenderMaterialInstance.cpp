#include "pch.h"
#include "Material/ZRenderMaterialInstance.hpp"
#include "Glacier2/Render/SRMaterialProperties.hpp"
#include "IO/ZBinaryReader.hpp"

namespace HM5ResourceTool
{
	ZRenderMaterialInstance::ZRenderMaterialInstance()
	{
		materialInfo = new IOI::SRMaterialProperties();
		instanceProperty = new ZProperty();
	}

	ZRenderMaterialInstance::~ZRenderMaterialInstance()
	{
		if (materialInfo)
		{
			delete materialInfo;
		}

		if (instanceProperty)
		{
			delete instanceProperty;
		}
	}

	IOI::SRMaterialProperties* ZRenderMaterialInstance::GetMaterialInfo()
	{
		return materialInfo;
	}

	ZRenderMaterialInstance::ZProperty* ZRenderMaterialInstance::GetInstanceProperty()
	{
		return instanceProperty;
	}

	const IOI::SRMaterialProperties* ZRenderMaterialInstance::GetMaterialInfo() const
	{
		return materialInfo;
	}

	const ZRenderMaterialInstance::ZProperty* ZRenderMaterialInstance::GetInstanceProperty() const
	{
		return instanceProperty;
	}

	void ZRenderMaterialInstance::Deserialize(const std::string& filePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(filePath);

		Deserialize(binaryReader);
	}

	void ZRenderMaterialInstance::Deserialize(const void* buffer, const unsigned int dataSize)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(buffer, dataSize);

		Deserialize(binaryReader);
	}

	void ZRenderMaterialInstance::Deserialize(IO::ZBinaryReader& binaryReader)
	{
		unsigned int materialInfoOffset = binaryReader.ReadUInt32();

		binaryReader.GetInputStream()->Seek(materialInfoOffset, IO::IInputStream::ESeekOrigin::Begin);
		binaryReader.ReadBytes(materialInfo, sizeof(IOI::SRMaterialProperties));

		unsigned int materialInstanceOffset = binaryReader.ReadUInt32();

		ReadProperty(instanceProperty, binaryReader, materialInstanceOffset);
	}

	std::string ZRenderMaterialInstance::SerializeToJSON()
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("materialInfo");
		writer.StartObject();

		writer.String("lMaterialClassType");
		writer.Uint(materialInfo->lMaterialClassType);
		writer.String("lMaterialEffectIndex");
		writer.Uint(materialInfo->lMaterialEffectIndex);
		writer.String("lMaterialClassFlags");
		writer.Uint(materialInfo->lMaterialClassFlags);
		writer.String("lTransparencyFlags");
		writer.Uint(materialInfo->lTransparencyFlags);
		writer.String("lMaterialDescriptor");
		writer.Uint(materialInfo->lMaterialDescriptor);
		writer.String("lImpactMaterial");
		writer.Uint(materialInfo->lImpactMaterial);
		writer.String("lEffectResource");
		writer.Uint(materialInfo->lEffectResource);

		writer.EndObject();

		writer.String("instanceProperty");
		SerializeProperty(instanceProperty, writer);

		return std::string();
	}

	void ZRenderMaterialInstance::SerializeProperty(ZProperty* property, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("propertyInfo");
		writer.StartObject();

		writer.String("lName");
		writer.Uint(property->GetPropertyInfo()->lName);
		writer.String("lData");
		writer.Uint(property->GetPropertyInfo()->lData);
		writer.String("lSize");
		writer.Uint(property->GetPropertyInfo()->lSize);
		writer.String("lType");
		writer.Uint(property->GetPropertyInfo()->lType);

		writer.EndObject();

		writer.String("childProperties");
		writer.StartArray();

		std::vector<ZProperty*>* childProperties = property->GetChildProperties();

		for (unsigned int i = 0; i < property->GetChildProperties()->size(); ++i)
		{
			SerializeProperty(childProperties->operator[](i), writer);
		}

		writer.EndArray();

		writer.EndObject();
	}

	void ZRenderMaterialInstance::ReadProperty(ZProperty* property, IO::ZBinaryReader& binaryReader, unsigned int propertyOffset)
	{
		binaryReader.GetInputStream()->Seek(propertyOffset, IO::IInputStream::ESeekOrigin::Begin);

		SProperty* propertyInfo = static_cast<SProperty*>(binaryReader.ReadBytes(sizeof(SProperty)));

		property->SetPropertyInfo(propertyInfo);

		switch (propertyInfo->lType)
		{
		case PT_FLOAT:
		{
			unsigned int value; //Value must be read as unsigned int because it's passed as byte array to C# code

			if (propertyInfo->lSize == 1)
			{
				value = propertyInfo->lData;
			}
			else
			{
				binaryReader.GetInputStream()->Seek(propertyInfo->lData, IO::IInputStream::ESeekOrigin::Begin);

				value = binaryReader.ReadUInt32();
			}

			void* data = operator new(sizeof(unsigned int));

			memcpy(data, &value, sizeof(unsigned int));

			property->SetValue(data);
			property->SetValueSize(sizeof(unsigned int));

			break;
		}
		case PT_CHAR:
		{
			binaryReader.GetInputStream()->Seek(propertyInfo->lData, IO::IInputStream::ESeekOrigin::Begin);

			std::string value = binaryReader.ReadString();
			void* data = operator new(value.length() + 1);

			memcpy(data, value.c_str(), value.length() + 1);

			property->SetValue(data);
			property->SetValueSize(value.length() + 1);

			break;
		}
		case PT_UINT32:
		{
			unsigned int value;

			if (propertyInfo->lSize == 1)
			{
				value = propertyInfo->lData;
			}
			else
			{
				binaryReader.GetInputStream()->Seek(propertyInfo->lData, IO::IInputStream::ESeekOrigin::Begin);

				value = binaryReader.ReadUInt32();
			}

			void* data = operator new(sizeof(unsigned int));

			memcpy(data, &value, sizeof(unsigned int));

			property->SetValue(data);
			property->SetValueSize(sizeof(unsigned int));

			break;
		}
		case PT_LIST:
		{
			unsigned childPropertyOffset = propertyInfo->lData;

			for (unsigned int i = 0; i < propertyInfo->lSize; ++i)
			{
				ZProperty* childProperty = new ZProperty();

				ReadProperty(childProperty, binaryReader, childPropertyOffset);

				property->AddChildProperty(childProperty);

				childPropertyOffset += sizeof(SProperty);
			}

			break;
		}
		}
	}

	ZRenderMaterialInstance::ZProperty::ZProperty()
	{
		propertyInfo = nullptr;
		value = nullptr;
		valueSize = 0;
	}

	ZRenderMaterialInstance::ZProperty::~ZProperty()
	{
		if (propertyInfo)
		{
			delete propertyInfo;
		}

		if (value)
		{
			delete value;
		}
	}

	ZRenderMaterialInstance::SProperty* ZRenderMaterialInstance::ZProperty::GetPropertyInfo()
	{
		return propertyInfo;
	}

	void* ZRenderMaterialInstance::ZProperty::GetValue()
	{
		return value;
	}

	unsigned int ZRenderMaterialInstance::ZProperty::GetValueSize()
	{
		return valueSize;
	}

	std::vector<ZRenderMaterialInstance::ZProperty*>* ZRenderMaterialInstance::ZProperty::GetChildProperties()
	{
		return &childProperties;
	}

	const ZRenderMaterialInstance::SProperty* ZRenderMaterialInstance::ZProperty::GetPropertyInfo() const
	{
		return propertyInfo;
	}

	const void* ZRenderMaterialInstance::ZProperty::GetValue() const
	{
		return value;
	}

	const std::vector<ZRenderMaterialInstance::ZProperty*>* ZRenderMaterialInstance::ZProperty::GetChildProperties() const
	{
		return &childProperties;
	}

	void ZRenderMaterialInstance::ZProperty::SetPropertyInfo(SProperty* propertyInfo)
	{
		this->propertyInfo = propertyInfo;
	}

	void ZRenderMaterialInstance::ZProperty::SetValue(void* value)
	{
		this->value = value;
	}

	void ZRenderMaterialInstance::ZProperty::SetValueSize(unsigned int valueSize)
	{
		this->valueSize = valueSize;
	}

	void ZRenderMaterialInstance::ZProperty::AddChildProperty(ZProperty* childProperty)
	{
		childProperties.push_back(childProperty);
	}
}
