#include "pch.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "Material/ZRenderMaterialEntity.hpp"
#include "Glacier2/Math/SVector4.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/SColorRGBA.hpp"

namespace HM5ResourceTool
{
	const std::vector<ZRenderMaterialEntity::SModifier*>& ZRenderMaterialEntity::GetModifiers()
	{
		return modifiers;
	}

	void ZRenderMaterialEntity::DeserializeEntityData(const std::string& mattFilePath)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(mattFilePath);

		DeserializeEntityData(binaryReader);
	}

	void ZRenderMaterialEntity::DeserializeEntityData(const void* data, const unsigned int dataSize)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(data, dataSize);

		DeserializeEntityData(binaryReader);
	}

	void ZRenderMaterialEntity::DeserializeEntityData(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
	{
		while (binaryReader.GetInputStream()->GetPosition() < binaryReader.GetInputStream()->GetSize())
		{
			SModifier* modifier = new SModifier();

			modifier->type = binaryReader.ReadByte();

			switch (modifier->type)
			{
			case 1:
				modifier->value = binaryReader.ReadBytes(sizeof(unsigned int));
				break;
			case 2:
				modifier->value = binaryReader.ReadBytes(sizeof(IOI::SColorRGB));
				break;
			case 3:
				modifier->value = binaryReader.ReadBytes(sizeof(IOI::SColorRGBA));
				break;
			case 4:
				modifier->value = binaryReader.ReadBytes(sizeof(float));
				break;
			case 5:
				modifier->value = binaryReader.ReadBytes(sizeof(IOI::SVector2));
				break;
			case 6:
				modifier->value = binaryReader.ReadBytes(sizeof(IOI::SVector3));
				break;
			case 7:
				modifier->value = binaryReader.ReadBytes(sizeof(IOI::SVector4));
				break;
			default:
				break;
			}

			modifiers.push_back(modifier);
		}
	}

	std::string ZRenderMaterialEntity::SerializeToJSON()
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("modifiers");
		writer.StartArray();

		for (unsigned int i = 0; i < modifiers.size(); ++i)
		{
			writer.StartObject();

			switch (modifiers[i]->type)
			{
			case 1:
				writer.String("type");
				writer.String("UInt32");
				writer.String("value");
				writer.Uint(*static_cast<unsigned int*>(modifiers[i]->value));
				break;
			case 2:
				writer.String("type");
				writer.String("SColorRGB");
				writer.String("value");
				static_cast<IOI::SColorRGB*>(modifiers[i]->value)->SerializeToJSON(writer);
				break;
			case 3:
				writer.String("type");
				writer.String("SColorRGBA");
				writer.String("value");
				static_cast<IOI::SColorRGBA*>(modifiers[i]->value)->SerializeToJSON(writer);
				break;
			case 4:
				writer.String("type");
				writer.String("Float");
				writer.String("value");
				writer.Double(*static_cast<float*>(modifiers[i]->value));
				break;
			case 5:
				writer.String("type");
				writer.String("SVector2");
				writer.String("value");
				static_cast<IOI::SVector2*>(modifiers[i]->value)->SerializeToJSON(writer);
				break;
			case 6:
				writer.String("type");
				writer.String("SVector3");
				writer.String("value");
				static_cast<IOI::SVector3*>(modifiers[i]->value)->SerializeToJSON(writer);
				break;
			case 7:
				writer.String("type");
				writer.String("SVector4");
				writer.String("value");
				static_cast<IOI::SVector4*>(modifiers[i]->value)->SerializeToJSON(writer);
				break;
			default:
				break;
			}

			writer.EndObject();
		}

		writer.EndArray();
		writer.EndObject();

		return stringBuffer.GetString();
	}
}
