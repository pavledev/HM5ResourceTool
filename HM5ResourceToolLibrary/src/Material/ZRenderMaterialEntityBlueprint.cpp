#include "pch.h"
#include "Material/ZRenderMaterialEntityBlueprint.hpp"

namespace HM5ResourceTool
{
	const std::vector<ZRenderMaterialEntityBlueprint::SModifier*>& ZRenderMaterialEntityBlueprint::GetModifiers()
	{
		return modifiers;
	}

	void ZRenderMaterialEntityBlueprint::DeserializeEntityData(const std::string& mattFilePath)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(mattFilePath);

		DeserializeEntityData(binaryReader);
	}

	void ZRenderMaterialEntityBlueprint::DeserializeEntityData(const void* data, const unsigned int dataSize)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(data, dataSize);

		DeserializeEntityData(binaryReader);
	}

	void ZRenderMaterialEntityBlueprint::DeserializeEntityData(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
	{
		while (binaryReader.GetInputStream()->GetPosition() < binaryReader.GetInputStream()->GetSize())
		{
			SModifier* modifier = new SModifier();

			modifier->type = binaryReader.ReadByte();
			modifier->name = binaryReader.ReadString(binaryReader.ReadUInt32() - 1);

			modifiers.push_back(modifier);
		}
	}

	std::string ZRenderMaterialEntityBlueprint::SerializeToJSON()
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("modifiers");
		writer.StartArray();

		for (unsigned int i = 0; i < modifiers.size(); ++i)
		{
			writer.StartObject();

			writer.String("type");

			switch (modifiers[i]->type)
			{
			case 1:
				writer.String("UInt32");
				break;
			case 2:
				writer.String("SColorRGB");
				break;
			case 3:
				writer.String("SColorRGBA");
				break;
			case 4:
				writer.String("Float");
				break;
			case 5:
				writer.String("SVector2");
				break;
			case 6:
				writer.String("SVector3");
				break;
			case 7:
				writer.String("SVector4");
				break;
			default:
				break;
			}

			writer.String("name");
			writer.String(modifiers[i]->name.c_str());

			writer.EndObject();
		}

		writer.EndArray();
		writer.EndObject();

		return stringBuffer.GetString();
	}
}
