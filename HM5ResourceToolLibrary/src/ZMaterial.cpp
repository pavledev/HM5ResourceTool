#include "pch.h"
#include <iostream>
#include "ZMaterial.hpp"
#include "Material/ZRenderMaterialInstance.hpp"
#include "Material/ZRenderMaterialEntity.hpp"
#include "Material/ZRenderMaterialEntityBlueprint.hpp"
#include "Glacier2/Math/SVector4.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/SColorRGBA.hpp"

namespace HM5ResourceTool
{
	bool ZMaterial::ConvertRenderMaterialInstanceToJSON(const std::string& matiPath, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialInstance renderMaterialInstance = ZRenderMaterialInstance();

			renderMaterialInstance.Deserialize(matiPath);

			jsonOutput = renderMaterialInstance.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialInstanceToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialInstance renderMaterialInstance = ZRenderMaterialInstance();

			renderMaterialInstance.Deserialize(data, dataSize);

			jsonOutput = renderMaterialInstance.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialEntityToJSON(const std::string& mattPath, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialEntity renderMaterialEntity = ZRenderMaterialEntity();

			renderMaterialEntity.DeserializeEntityData(mattPath);

			jsonOutput = renderMaterialEntity.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialEntityToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialEntity renderMaterialEntity = ZRenderMaterialEntity();

			renderMaterialEntity.DeserializeEntityData(data, dataSize);

			jsonOutput = renderMaterialEntity.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(const std::string& matbPath, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialEntityBlueprint renderMaterialEntityBlueprint = ZRenderMaterialEntityBlueprint();

			renderMaterialEntityBlueprint.DeserializeEntityData(matbPath);

			jsonOutput = renderMaterialEntityBlueprint.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialEntityBlueprint renderMaterialEntityBlueprint = ZRenderMaterialEntityBlueprint();

			renderMaterialEntityBlueprint.DeserializeEntityData(data, dataSize);

			jsonOutput = renderMaterialEntityBlueprint.SerializeToJSON();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZMaterial::ConvertRenderMaterialEntityAndBlueprintToJSON(const ZResource* mattResource, std::string& jsonOutput)
	{
		try
		{
			ZRenderMaterialEntity renderMaterialEntity = ZRenderMaterialEntity();
			ZRenderMaterialEntityBlueprint renderMaterialEntityBlueprint = ZRenderMaterialEntityBlueprint();

			const unsigned int matbReferenceIndex = GetIndexOfMaterialEntityBlueprintReference(mattResource);
			const void* mattData = mattResource->GetResourceData();
			const void* matbData = mattResource->GetReferences()->operator[](matbReferenceIndex)->GetResourceData();
			const unsigned int mattDataSize = mattResource->GetResourceDataSize();
			const unsigned int matbDataSize = mattResource->GetReferences()->operator[](matbReferenceIndex)->GetResourceDataSize();

			renderMaterialEntity.DeserializeEntityData(mattData, mattDataSize);
			renderMaterialEntityBlueprint.DeserializeEntityData(matbData, matbDataSize);

			const std::vector<ZRenderMaterialEntity::SModifier*> mattModifiers = renderMaterialEntity.GetModifiers();
			const std::vector<ZRenderMaterialEntityBlueprint::SModifier*> matbModifiers = renderMaterialEntityBlueprint.GetModifiers();

			rapidjson::StringBuffer stringBuffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

			writer.StartObject();

			writer.String("modifiers");
			writer.StartArray();

			for (unsigned int i = 0; i < mattModifiers.size(); ++i)
			{
				writer.StartObject();

				writer.String("name");
				writer.String(matbModifiers[i]->name.c_str());

				switch (mattModifiers[i]->type)
				{
				case 1:
					writer.String("type");
					writer.String("UInt32");
					writer.String("value");
					writer.Uint(*static_cast<unsigned int*>(mattModifiers[i]->value));
					break;
				case 2:
					writer.String("type");
					writer.String("SColorRGB");
					writer.String("value");
					static_cast<IOI::SColorRGB*>(mattModifiers[i]->value)->SerializeToJSON(writer);
					break;
				case 3:
					writer.String("type");
					writer.String("SColorRGBA");
					writer.String("value");
					static_cast<IOI::SColorRGBA*>(mattModifiers[i]->value)->SerializeToJSON(writer);
					break;
				case 4:
					writer.String("type");
					writer.String("Float");
					writer.String("value");
					writer.Double(*static_cast<float*>(mattModifiers[i]->value));
					break;
				case 5:
					writer.String("type");
					writer.String("SVector2");
					writer.String("value");
					static_cast<IOI::SVector2*>(mattModifiers[i]->value)->SerializeToJSON(writer);
					break;
				case 6:
					writer.String("type");
					writer.String("SVector3");
					writer.String("value");
					static_cast<IOI::SVector3*>(mattModifiers[i]->value)->SerializeToJSON(writer);
					break;
				case 7:
					writer.String("type");
					writer.String("SVector4");
					writer.String("value");
					static_cast<IOI::SVector4*>(mattModifiers[i]->value)->SerializeToJSON(writer);
					break;
				default:
					break;
				}

				writer.EndObject();
			}

			writer.EndArray();
			writer.EndObject();

			jsonOutput = stringBuffer.GetString();
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	unsigned int ZMaterial::GetIndexOfMaterialEntityBlueprintReference(const ZResource* mattResource)
	{
		const std::vector<ZResource*>* references = mattResource->GetReferences();

		for (unsigned int i = 0; i < references->size(); ++i)
		{
			if (references->operator[](i)->GetFileFormat() == "MATB")
			{
				return i;
			}
		}

		return 0;
	}
}
