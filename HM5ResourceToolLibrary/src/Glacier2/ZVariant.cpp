#include "pch.h"
#include <format>
#include "Glacier2/ZVariant.hpp"
#include "Glacier2/ZBinarySerializer.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/Math/SMatrix43.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/SColorRGBA.hpp"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/TArray.hpp"
#include "Glacier2/ZCurve.hpp"
#include "Glacier2/ZGameTime.hpp"
#include "Glacier2/ZSharedSensorDef.hpp"
#include "Glacier2/Sound/ZSpeakerLevels.hpp"
#include "ECameraState.hpp"
#include "IO/ZJSONDeserializer.hpp"
#include "Glacier2/SBodyPartDamageMultipliers.hpp"
#include "Glacier2/SSettingsParamMultiplier.hpp"

namespace IOI
{
	ZVariant::ZVariant() : ZObjectRef()
	{

	}

	//ZVariant::~ZVariant()
	//{
	//	//Type ids will be deleted from ZTypeRegistry destructor
	//	if (m_TypeID)
	//	{
	//		operator delete(m_TypeID);
	//	}

	//	if (m_pData)
	//	{
	//		delete m_pData;

	//		m_pData = nullptr;
	//	}
	//}

	void ZVariant::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("typeName");
		m_TypeID->SerializeToJSON(writer);

		writer.String("value");

		std::string typeName = m_TypeID->pTypeInfo->pszTypeName;

		if (!SerializeData(writer))
		{
			throw std::invalid_argument(std::format("Type {} isn't supported", typeName));
		}

		writer.EndObject();
	}

	void ZVariant::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
	{
		unsigned int typeIDOffset = offset + offsetof(ZVariant, m_TypeID);
		unsigned int dataOffset = offset + offsetof(ZVariant, m_pData);
		unsigned int nullPointer = -1;

		binarySerializer.RecordOffsetForTypeIDReindexing(typeIDOffset);

		if (m_TypeID)
		{
			unsigned int typeIndex = binarySerializer.TypeIdToIndex(m_TypeID);

			binarySerializer.WriteToMemory(&typeIndex, sizeof(unsigned int), typeIDOffset);

			if (m_pData)
			{
				std::optional<unsigned int> result = binarySerializer.GetDataOffset(this);
				unsigned int dataOffset2;

				if (result.has_value())
				{
					dataOffset2 = result.value();
				}
				else
				{
					unsigned int typeSize = m_TypeID->pTypeInfo->GetTypeSize();
					unsigned int typeAlignment = m_TypeID->pTypeInfo->GetTypeAlignment();

					dataOffset2 = binarySerializer.GetAlignedOffset(typeSize, typeAlignment);

					std::string typeName = m_TypeID->pTypeInfo->pszTypeName;

					if (m_TypeID->pTypeInfo->IsPrimitiveType() || m_TypeID->pTypeInfo->IsEnumType())
					{
						binarySerializer.WriteToMemory(m_pData, typeSize, dataOffset2);
					}
					else if (typeName == "SVector2")
					{
						static_cast<SVector2*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SVector3")
					{
						static_cast<SVector3*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SMatrix43")
					{
						static_cast<SMatrix43*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SColorRGB")
					{
						static_cast<SColorRGB*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SColorRGBA")
					{
						static_cast<SColorRGBA*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SEntityTemplateReference")
					{
						static_cast<SEntityTemplateReference*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "SBodyPartDamageMultipliers")
					{
						static_cast<SBodyPartDamageMultipliers*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "ZRuntimeResourceID")
					{
						static_cast<ZRuntimeResourceID*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "ZString")
					{
						static_cast<ZString*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "ZCurve")
					{
						static_cast<ZCurve*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "ZGameTime")
					{
						static_cast<ZGameTime*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "ZSpeakerLevels")
					{
						static_cast<ZSpeakerLevels*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<SEntityTemplateReference>")
					{
						static_cast<TArray<SEntityTemplateReference>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<float32>")
					{
						static_cast<TArray<float>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<ZGameTime>")
					{
						static_cast<TArray<ZGameTime>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<SVector2>")
					{
						static_cast<TArray<SVector2>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<bool>")
					{
						static_cast<TArray<bool>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<ZSharedSensorDef.SVisibilitySetting>")
					{
						static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<ZString>")
					{
						static_cast<TArray<ZString>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<SSettingsParamMultiplier>")
					{
						static_cast<TArray<SSettingsParamMultiplier>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<SColorRGB>")
					{
						static_cast<TArray<SColorRGB>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}
					else if (typeName == "TArray<ECameraState>")
					{
						static_cast<TArray<ECameraState>*>(m_pData)->SerializeToMemory(binarySerializer, dataOffset2);
					}

					binarySerializer.SetDataOffset(this, dataOffset2);
				}

				binarySerializer.WriteToMemory(&dataOffset2, sizeof(unsigned int), dataOffset);
			}
			else
			{
				binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), dataOffset);
			}
		}
		else
		{
			binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), typeIDOffset);
			binarySerializer.WriteToMemory(&nullPointer, sizeof(unsigned int), dataOffset);
		}

		binarySerializer.RecordOffsetForRebasing(dataOffset);
	}

	bool ZVariant::SerializeData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		std::string typeName = m_TypeID->pTypeInfo->pszTypeName;

		if (typeName == "bool")
		{
			writer.Bool(*static_cast<bool*>(m_pData));
		}
		else if (typeName == "int8" || typeName == "char")
		{
			writer.Int(*static_cast<char*>(m_pData));
		}
		else if (typeName == "uint8")
		{
			writer.Uint(*static_cast<unsigned char*>(m_pData));
		}
		else if (typeName == "int16")
		{
			writer.Int(*static_cast<short*>(m_pData));
		}
		else if (typeName == "uint16")
		{
			writer.Uint(*static_cast<unsigned short*>(m_pData));
		}
		else if (typeName == "int32")
		{
			writer.Int(*static_cast<int*>(m_pData));
		}
		else if (typeName == "uint32")
		{
			writer.Uint(*static_cast<unsigned int*>(m_pData));
		}
		else if (typeName == "int64")
		{
			writer.Int64(*static_cast<long long*>(m_pData));
		}
		else if (typeName == "uint64")
		{
			writer.Uint64(*static_cast<unsigned long long*>(m_pData));
		}
		else if (typeName == "float32")
		{
			writer.Double(*static_cast<float*>(m_pData));
		}
		else if (typeName == "float64")
		{
			writer.Double(*static_cast<double*>(m_pData));
		}
		else if (typeName == "SVector2")
		{
			static_cast<SVector2*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SVector3")
		{
			static_cast<SVector3*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SMatrix43")
		{
			static_cast<SMatrix43*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SColorRGB")
		{
			static_cast<SColorRGB*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SColorRGBA")
		{
			static_cast<SColorRGBA*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SEntityTemplateReference")
		{
			static_cast<SEntityTemplateReference*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "SBodyPartDamageMultipliers")
		{
			static_cast<SBodyPartDamageMultipliers*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "ZRuntimeResourceID")
		{
			static_cast<ZRuntimeResourceID*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "ZString")
		{
			static_cast<ZString*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "ZCurve")
		{
			static_cast<ZCurve*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "ZGameTime")
		{
			static_cast<ZGameTime*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "ZSpeakerLevels")
		{
			static_cast<ZSpeakerLevels*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<SEntityTemplateReference>")
		{
			static_cast<TArray<SEntityTemplateReference>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<float32>")
		{
			static_cast<TArray<float>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<ZGameTime>")
		{
			static_cast<TArray<ZGameTime>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<SVector2>")
		{
			static_cast<TArray<SVector2>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<bool>")
		{
			static_cast<TArray<bool>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<ZSharedSensorDef.SVisibilitySetting>")
		{
			static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<ZString>")
		{
			static_cast<TArray<ZString>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<SSettingsParamMultiplier>")
		{
			static_cast<TArray<SSettingsParamMultiplier>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<SColorRGB>")
		{
			static_cast<TArray<SColorRGB>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (typeName == "TArray<ECameraState>")
		{
			static_cast<TArray<ECameraState>*>(m_pData)->SerializeToJSON(writer);
		}
		else if (m_TypeID->pTypeInfo->IsEnumType())
		{
			if (enumRegistry.IsEmpty())
			{
				LoadEnumRegistry();
			}

			std::map<std::string, int> items = enumRegistry.GetEnumByTypeName(typeName);

			if (items.size() == 0)
			{
				return false;
			}

			int value = *static_cast<int*>(m_pData);

			for (auto it = items.begin(); it != items.end(); it++)
			{
				if (it->second == value)
				{
					writer.String(it->first.c_str());

					break;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	ZVariant* ZVariant::DeserializeFromJSON(const rapidjson::Value& object)
	{
		ZVariant* variant = new ZVariant();
		std::string typeName = object["typeName"].GetString();
		static std::unordered_map<STypeID*, std::vector<void*>> values;

		variant->m_TypeID = STypeID::DeserializeFromJSON(object, "typeName");

		if (enumRegistry.IsEmpty())
		{
			LoadEnumRegistry();
		}

		if (typeName == "bool")
		{
			bool value2 = object["value"].GetBool();

			variant->m_pData = operator new(sizeof(bool));

			memcpy(variant->m_pData, &value2, sizeof(bool));
		}
		else if (typeName == "int8" || typeName == "char")
		{
			char value = object["value"].GetInt();

			variant->m_pData = operator new(sizeof(char));

			memcpy(variant->m_pData, &value, sizeof(char));
		}
		else if (typeName == "uint8")
		{
			unsigned char value = object["value"].GetUint();

			variant->m_pData = operator new(sizeof(unsigned char));

			memcpy(variant->m_pData, &value, sizeof(unsigned char));
		}
		else if (typeName == "int16")
		{
			short value = object["value"].GetInt();

			variant->m_pData = operator new(sizeof(short));

			memcpy(variant->m_pData, &value, sizeof(short));
		}
		else if (typeName == "uint16")
		{
			unsigned short value = object["value"].GetUint();

			variant->m_pData = operator new(sizeof(unsigned short));

			memcpy(variant->m_pData, &value, sizeof(unsigned short));
		}
		else if (typeName == "int32")
		{
			int value = object["value"].GetInt();

			variant->m_pData = operator new(sizeof(int));

			memcpy(variant->m_pData, &value, sizeof(int));
		}
		else if (typeName == "uint32")
		{
			unsigned int value = object["value"].GetUint();

			variant->m_pData = operator new(sizeof(unsigned int));

			memcpy(variant->m_pData, &value, sizeof(unsigned int));
		}
		else if (typeName == "int64")
		{
			long long value = object["value"].GetInt64();

			variant->m_pData = operator new(sizeof(long long));

			memcpy(variant->m_pData, &value, sizeof(long long));
		}
		else if (typeName == "uint64")
		{
			unsigned long long value2 = object["value"].GetUint64();

			variant->m_pData = operator new(sizeof(unsigned long long));

			memcpy(variant->m_pData, &value2, sizeof(unsigned long long));
		}
		else if (typeName == "float32")
		{
			float value2 = object["value"].GetFloat();

			variant->m_pData = operator new(sizeof(float));

			memcpy(variant->m_pData, &value2, sizeof(float));
		}
		else if (typeName == "float64")
		{
			double value2 = object["value"].GetDouble();

			variant->m_pData = operator new(sizeof(double));

			memcpy(variant->m_pData, &value2, sizeof(double));
		}
		else if (typeName == "SVector2")
		{
			variant->m_pData = SVector2::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SVector3")
		{
			variant->m_pData = SVector3::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SMatrix43")
		{
			variant->m_pData = SMatrix43::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SColorRGB")
		{
			variant->m_pData = SColorRGB::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SColorRGBA")
		{
			variant->m_pData = SColorRGBA::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SEntityTemplateReference")
		{
			variant->m_pData = SEntityTemplateReference::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "SBodyPartDamageMultipliers")
		{
			variant->m_pData = SBodyPartDamageMultipliers::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "ZRuntimeResourceID")
		{
			variant->m_pData = ZRuntimeResourceID::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "ZString")
		{
			variant->m_pData = ZString::DeserializeFromJSON(object["value"].GetString());
		}
		else if (typeName == "ZCurve")
		{
			variant->m_pData = ZCurve::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "ZGameTime")
		{
			variant->m_pData = ZGameTime::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "ZSpeakerLevels")
		{
			variant->m_pData = ZSpeakerLevels::DeserializeFromJSON(object["value"].GetObj());
		}
		else if (typeName == "TArray<float32>")
		{
			variant->m_pData = TArray<float>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<SEntityTemplateReference>")
		{
			variant->m_pData = TArray<SEntityTemplateReference>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<ZGameTime>")
		{
			variant->m_pData = TArray<ZGameTime>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<SVector2>")
		{
			variant->m_pData = TArray<SVector2>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<bool>")
		{
			variant->m_pData = TArray<bool>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<ZSharedSensorDef.SVisibilitySetting>")
		{
			variant->m_pData = TArray<ZSharedSensorDef::SVisibilitySetting>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<ZString>")
		{
			variant->m_pData = TArray<ZString>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<SSettingsParamMultiplier>")
		{
			variant->m_pData = TArray<SSettingsParamMultiplier>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<SColorRGB>")
		{
			variant->m_pData = TArray<SColorRGB>::DeserializeFromJSON(object["value"].GetArray());
		}
		else if (typeName == "TArray<ECameraState>")
		{
			variant->m_pData = TArray<ECameraState>::DeserializeFromJSON(object["value"].GetArray());
		}
		else
		{
			std::string value = object["value"].GetString();
			std::map<std::string, int> items = enumRegistry.GetEnumByTypeName(typeName);

			variant->m_pData = operator new(sizeof(unsigned int));

			for (auto it = items.begin(); it != items.end(); it++)
			{
				if (it->first == value)
				{
					memcpy(variant->m_pData, &it->second, sizeof(unsigned int));

					break;
				}
			}
		}

		//Check whether SSettingsParamMultiplier, TArray<ZSharedSensorDef::SVisibilitySetting> and TArray<ECameraState> should be compared because of enum
		void* value = GetValue(variant, values);

		if (value)
		{
			variant->m_pData = value;
		}
		else
		{
			//SEntityTemplateReference objects and enums aren't duplicated
			if (typeName != "SEntityTemplateReference" &&
				typeName != "TArray<SEntityTemplateReference>" &&
				!variant->m_TypeID->pTypeInfo->IsEnumType())
			{
				values[variant->m_TypeID].push_back(variant->m_pData);
			}
		}

		return variant;
	}

	void* ZVariant::GetValue(const ZVariant* variant, const std::unordered_map<STypeID*, std::vector<void*>>& values)
	{
		auto it = values.find(variant->m_TypeID);
		void* result = nullptr;

		if (it != values.end())
		{
			std::string typeName = variant->m_TypeID->pTypeInfo->pszTypeName;
			bool found = false;
			int index = 0;

			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if ((typeName == "bool") &&
					*static_cast<bool*>(variant->m_pData) == *static_cast<bool*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "int8" || typeName == "char") &&
					*static_cast<char*>(variant->m_pData) == *static_cast<char*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "uint8") &&
					*static_cast<unsigned char*>(variant->m_pData) == *static_cast<unsigned char*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "int16") &&
					*static_cast<short*>(variant->m_pData) == *static_cast<short*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "uint16") &&
					*static_cast<unsigned short*>(variant->m_pData) == *static_cast<unsigned short*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "int32") &&
					*static_cast<int*>(variant->m_pData) == *static_cast<int*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "uint32") &&
					*static_cast<unsigned int*>(variant->m_pData) == *static_cast<unsigned int*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "int64") &&
					*static_cast<long long*>(variant->m_pData) == *static_cast<long long*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "uint64") &&
					*static_cast<unsigned long long*>(variant->m_pData) == *static_cast<unsigned long long*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "float32") &&
					*static_cast<float*>(variant->m_pData) == *static_cast<float*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "float64") &&
					*static_cast<double*>(variant->m_pData) == *static_cast<double*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "SVector2") &&
					*static_cast<SVector2*>(variant->m_pData) == *static_cast<SVector2*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "SVector3") &&
					*static_cast<SVector3*>(variant->m_pData) == *static_cast<SVector3*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "SMatrix43") &&
					*static_cast<SMatrix43*>(variant->m_pData) == *static_cast<SMatrix43*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "SColorRGB") &&
					*static_cast<SColorRGB*>(variant->m_pData) == *static_cast<SColorRGB*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "SColorRGBA") &&
					*static_cast<SColorRGBA*>(variant->m_pData) == *static_cast<SColorRGBA*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				/*else if ((typeName == "SEntityTemplateReference") &&
					*static_cast<SEntityTemplateReference*>(variant->m_pData) == *static_cast<SEntityTemplateReference*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}*/
				else if ((typeName == "SBodyPartDamageMultipliers") &&
					*static_cast<SBodyPartDamageMultipliers*>(variant->m_pData) == *static_cast<SBodyPartDamageMultipliers*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "ZRuntimeResourceID") &&
					*static_cast<ZRuntimeResourceID*>(variant->m_pData) == *static_cast<ZRuntimeResourceID*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "ZString") &&
					*static_cast<ZString*>(variant->m_pData) == *static_cast<ZString*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "ZCurve") &&
					*static_cast<ZCurve*>(variant->m_pData) == *static_cast<ZCurve*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "ZGameTime") &&
					*static_cast<ZGameTime*>(variant->m_pData) == *static_cast<ZGameTime*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "ZSpeakerLevels") &&
					*static_cast<ZSpeakerLevels*>(variant->m_pData) == *static_cast<ZSpeakerLevels*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				/*else if ((typeName == "TArray<SEntityTemplateReference>") &&
					*static_cast<TArray<SEntityTemplateReference>*>(variant->m_pData) == *static_cast<TArray<SEntityTemplateReference>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}*/
				else if ((typeName == "TArray<float32>") &&
					*static_cast<TArray<float>*>(variant->m_pData) == *static_cast<TArray<float>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "TArray<ZGameTime>") &&
					*static_cast<TArray<ZGameTime>*>(variant->m_pData) == *static_cast<TArray<ZGameTime>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "TArray<SVector2>") &&
					*static_cast<TArray<SVector2>*>(variant->m_pData) == *static_cast<TArray<SVector2>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				else if ((typeName == "TArray<bool>") &&
					*static_cast<TArray<bool>*>(variant->m_pData) == *static_cast<TArray<bool>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}
				/*else if ((typeName == "TArray<ZSharedSensorDef.SVisibilitySetting>") &&
					*static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(variant->m_pData) == *static_cast<TArray<ZSharedSensorDef::SVisibilitySetting>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}*/
				else if ((typeName == "TArray<ZString>") &&
					*static_cast<TArray<ZString>*>(variant->m_pData) == *static_cast<TArray<ZString>*>(it->second[i]))
				{
					found = true;
					index = i;

					break;
				}

				if (found)
				{
					result = it->second[index];
				}
			}
		}

		return result;
	}

	void ZVariant::LoadEnumRegistry()
	{
		char buffer[MAX_PATH];

		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		std::string filePath = std::format("{}/Enums.json", std::string(buffer).substr(0, pos));
		HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

		jsonDeserializer.Deserialize<ZEnumRegistry>(enumRegistry);
	}
}
