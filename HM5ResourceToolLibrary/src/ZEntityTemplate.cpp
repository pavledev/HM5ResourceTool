#include "pch.h"
#include <filesystem>
#include <algorithm>
#include <execution>
#include "ZEntityTemplate.hpp"
#include "ZModel.hpp"
#include "ZUtility.hpp"
#include <omp.h>

namespace HM5ResourceTool
{
	ZEntityTemplate::ZEntityTemplate()
	{
		if (propertyRegistry.IsEmpty())
		{
			char buffer[MAX_PATH];

			GetModuleFileNameA(NULL, buffer, MAX_PATH);

			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			std::string filePath = std::format("{}/Properties.json", std::string(buffer).substr(0, pos));
			HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

			jsonDeserializer.Deserialize<ZPropertyRegistry>(propertyRegistry);
		}
	}

	ZEntityTemplate::~ZEntityTemplate()
	{
		if (templateEntity)
		{
			operator delete(templateEntity, std::align_val_t(templateEntityAlignment));
		}

		if (templateEntityBlueprint)
		{
			operator delete(templateEntityBlueprint, std::align_val_t(templateEntityBlueprintAlignment));
		}
	}

	bool ZEntityTemplate::ConvertTemplateEntityToJSON(const std::vector<ZResource*>* references, const std::string& templateEntityPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::STemplateEntity* templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(templateEntityPath, references));

			/*IOI::ZBinarySerializer binarySerializer;
			std::unordered_map<unsigned long long, unsigned int> references2;

			for (unsigned int i = 0; i < references->size(); ++i)
			{
				references2.insert(std::make_pair(references->operator[](i)->GetRuntimeResourceID()->GetID(), i));
			}

			binarySerializer.SerializeToFile(templateEntity, "SERIALIZED_TEMP.TEMP", &references2);*/
			jsonOutput = templateEntity->SerializeToJSON();

			operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertTemplateEntityToJSON(const std::vector<ZResource*>* references, const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::STemplateEntity* templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(data, dataSize, references));
			jsonOutput = templateEntity->SerializeToJSON();

			/*IOI::ZBinarySerializer binarySerializer;
			std::unordered_map<unsigned long long, unsigned int> references2;

			for (unsigned int i = 0; i < references->size(); ++i)
			{
				references2.insert(std::make_pair(references->operator[](i)->GetRuntimeResourceID()->GetID(), i));
			}

			binarySerializer.SerializeToFile(templateEntity, "SERIALIZED_TEMP.TEMP", &references2);*/

			operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(const std::string& templateEntityBlueprintPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::STemplateEntityBlueprint* templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(templateEntityBlueprintPath));

			//IOI::ZBinarySerializer binarySerializer;
			//binarySerializer.SerializeToFile(templateEntityBlueprint, "SERIALIZED_TBLU.TBLU");

			jsonOutput = templateEntityBlueprint->SerializeToJSON();

			operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::STemplateEntityBlueprint* templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(data, dataSize));

			jsonOutput = templateEntityBlueprint->SerializeToJSON();

			operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertCppEntityToJSON(const std::vector<ZResource*>* references, const std::string& cppEntityPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SCppEntity* cppEntity = static_cast<IOI::SCppEntity*>(binaryDeserializer.Deserialize(cppEntityPath));

			std::unordered_map<unsigned long long, unsigned int> references2;

			for (unsigned int i = 0; i < references->size(); ++i)
			{
				references2.insert(std::make_pair(references->operator[](i)->GetRuntimeResourceID()->GetID(), i));
			}

			IOI::ZBinarySerializer binarySerializer;
			binarySerializer.SerializeToFile(cppEntity, "SERIALIZED_CPPT.CPPT", &references2);

			jsonOutput = cppEntity->SerializeToJSON();

			operator delete(cppEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertCppEntityToJSON(const std::vector<ZResource*>* references, const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SCppEntity* cppEntity = static_cast<IOI::SCppEntity*>(binaryDeserializer.Deserialize(data, dataSize));

			jsonOutput = cppEntity->SerializeToJSON();

			operator delete(cppEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertCppEntityBlueprintToJSON(const std::string& cppEntityBlueprintPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SCppEntityBlueprint* cppEntityBlueprint = static_cast<IOI::SCppEntityBlueprint*>(binaryDeserializer.Deserialize(cppEntityBlueprintPath));

			jsonOutput = cppEntityBlueprint->SerializeToJSON();

			operator delete(cppEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertCppEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SCppEntityBlueprint* cppEntityBlueprint = static_cast<IOI::SCppEntityBlueprint*>(binaryDeserializer.Deserialize(data, dataSize));

			jsonOutput = cppEntityBlueprint->SerializeToJSON();

			operator delete(cppEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	bool ZEntityTemplate::ConvertJSONToTemplateEntity(const std::string& jsonPath, const std::vector<ZResource*>* references, void** data, unsigned int& dataSize)
	{
		try
		{
			IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(jsonPath);
			IOI::STemplateEntity* templateEntity = jsonDeserializer.Deserialize<IOI::STemplateEntity>();
			IOI::ZBinarySerializer binarySerializer;
			std::unordered_map<unsigned long long, unsigned int> references2;

			for (unsigned int i = 0; i < references->size(); ++i)
			{
				references2.insert(std::make_pair(references->operator[](i)->GetRuntimeResourceID()->GetID(), i));
			}

			binarySerializer.SerializeToFile(templateEntity, "SERIALIZED_FROM_JSON.TEMP", &references2);

			binarySerializer.SerializeToMemory(templateEntity, data, dataSize, &references2);

			delete templateEntity;
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

	bool ZEntityTemplate::ConvertJSONToTemplateEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize)
	{
		try
		{
			IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(jsonPath);
			IOI::STemplateEntityBlueprint* templateEntityBlueprint = jsonDeserializer.Deserialize<IOI::STemplateEntityBlueprint>();
			IOI::ZBinarySerializer binarySerializer;

			binarySerializer.SerializeToMemory(templateEntityBlueprint, data, dataSize);

			delete templateEntityBlueprint;
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

	bool ZEntityTemplate::ConvertJSONToCppEntity(const std::string& jsonPath, void** data, unsigned int& dataSize)
	{
		try
		{
			IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(jsonPath);
			IOI::SCppEntity* cppEntity = jsonDeserializer.Deserialize<IOI::SCppEntity>();
			IOI::ZBinarySerializer binarySerializer;

			binarySerializer.SerializeToMemory(cppEntity, data, dataSize);

			delete cppEntity;
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

	bool ZEntityTemplate::ConvertJSONToCppEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize)
	{
		try
		{
			IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(jsonPath);
			IOI::SCppEntityBlueprint* cppEntityBlueprint = jsonDeserializer.Deserialize<IOI::SCppEntityBlueprint>();
			IOI::ZBinarySerializer binarySerializer;

			binarySerializer.SerializeToMemory(cppEntityBlueprint, data, dataSize);

			delete cppEntityBlueprint;
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

	bool ZEntityTemplate::ConvertTEMPAndTBLUToJSON(const ZResource* resource, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			const void* tempData = resource->GetResourceData();
			const unsigned int tempDataSize = resource->GetResourceDataSize();
			IOI::STemplateEntity* templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(tempData, tempDataSize, resource->GetReferences()));

			ZResource* tbluReference = GetTemplateEntityBlueprintReference(resource, templateEntity->blueprintIndexInResourceHeader);

			if (!tbluReference)
			{
				return false;
			}

			const void* tbluData = tbluReference->GetResourceData();
			const unsigned int tbluDataSize = tbluReference->GetResourceDataSize();

			IOI::STemplateEntityBlueprint* templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(tbluData, tbluDataSize));

			rapidjson::StringBuffer stringBuffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

			writer.StartObject();

			writer.String("blueprintIndexInResourceHeader");
			writer.Int(templateEntity->blueprintIndexInResourceHeader);

			writer.String("rootEntityIndex");
			writer.Int(templateEntity->rootEntityIndex);

			writer.String("entityTemplates");
			writer.StartArray();

			for (unsigned int i = 0; i < templateEntity->entityTemplates.Size(); ++i)
			{
				writer.StartObject();

				writer.String("parentIndex");
				writer.Int(templateEntity->entityTemplates[i].parentIndex);

				writer.String("entityTypeResourceIndex");
				writer.Int(templateEntity->entityTemplates[i].entityTypeResourceIndex);

				writer.String("entityName");
				writer.String(templateEntityBlueprint->entityTemplates[i].entityName.ToCString());

				writer.String("propertyValues");
				templateEntity->entityTemplates[i].propertyValues.SerializeToJSON(writer);

				writer.String("postInitPropertyValues");
				templateEntity->entityTemplates[i].postInitPropertyValues.SerializeToJSON(writer);

				writer.String("propertyAliases");
				templateEntityBlueprint->entityTemplates[i].propertyAliases.SerializeToJSON(writer);

				writer.String("exposedEntities");
				templateEntityBlueprint->entityTemplates[i].exposedEntities.SerializeToJSON(writer);

				writer.String("exposedInterfaces");
				templateEntityBlueprint->entityTemplates[i].exposedInterfaces.SerializeToJSON(writer);

				writer.String("entitySubsets");
				templateEntityBlueprint->entityTemplates[i].entitySubsets.SerializeToJSON(writer);

				writer.EndObject();
			}

			writer.EndArray();

			writer.String("pinConnections");
			templateEntityBlueprint->pinConnections.SerializeToJSON(writer);

			writer.String("inputPinForwardings");
			templateEntityBlueprint->inputPinForwardings.SerializeToJSON(writer);

			writer.String("outputPinForwardings");
			templateEntityBlueprint->outputPinForwardings.SerializeToJSON(writer);

			writer.EndObject();

			jsonOutput = stringBuffer.GetString();

			operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
			operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
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

	SEntityTemplate* ZEntityTemplate::JoinTEMPAndTBLU(const ZResource* resource, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary)
	{
		SEntityTemplate* entityTemplate = new SEntityTemplate();
		IOI::ZBinaryDeserializer binaryDeserializer;
		const void* tempData = resource->GetResourceData();
		const unsigned int tempDataSize = resource->GetResourceDataSize();

		templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(tempData, tempDataSize, resource->GetReferences()));
		templateEntityAlignment = binaryDeserializer.GetAlignment();

		ZResource* tbluReference = GetTemplateEntityBlueprintReference(resource, templateEntity->blueprintIndexInResourceHeader);

		if (!tbluReference->IsResourceDataLoaded())
		{
			std::vector<ZResourceLibrary*>* resourceLibraries = headerLibrary->GetResourceLibraries();
			unsigned int resourceLibraryIndex = headerLibrary->GetIndexOfResourceLibrary(*tbluReference->GetRuntimeResourceID(), false);
			unsigned int resourceIndex = headerLibrary->GetIndexOfResource(*tbluReference->GetRuntimeResourceID());

			resourceLibraries->operator[](resourceLibraryIndex)->LoadResourceData(resourceIndex);
			headerLibrary->LoadResourceReferences(resourceLibraryIndex, resourceIndex, globalHeaderLibrary);
		}

		const void* tbluData = tbluReference->GetResourceData();
		const unsigned int tbluDataSize = tbluReference->GetResourceDataSize();

		templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(tbluData, tbluDataSize));
		templateEntityBlueprintAlignment = binaryDeserializer.GetAlignment();

		entityTemplate->blueprintIndexInResourceHeader = templateEntity->blueprintIndexInResourceHeader;
		entityTemplate->rootEntityIndex = templateEntity->rootEntityIndex;

		for (unsigned i = 0; i < templateEntity->entityTemplates.Size(); ++i)
		{
			if (templateEntity->entityTemplates[i].parentIndex == -1)
			{
				SEntityTemplate::SSubEntityTemplate* rootEntityTemplate = new SEntityTemplate::SSubEntityTemplate();

				rootEntityTemplate->templateEntityResourceID = resource->GetResourceID();
				rootEntityTemplate->templateEntityBlueprintResourceID = tbluReference->GetResourceID();
				rootEntityTemplate->index = i;
				rootEntityTemplate->parentIndex = templateEntity->entityTemplates[i].parentIndex;
				rootEntityTemplate->entityTypeResourceIndex = templateEntity->entityTemplates[i].entityTypeResourceIndex;
				rootEntityTemplate->entityName = templateEntityBlueprint->entityTemplates[i].entityName;
				rootEntityTemplate->propertyValues = templateEntity->entityTemplates[i].propertyValues;
				rootEntityTemplate->postInitPropertyValues = templateEntity->entityTemplates[i].postInitPropertyValues;
				rootEntityTemplate->propertyAliases = templateEntityBlueprint->entityTemplates[i].propertyAliases;
				rootEntityTemplate->exposedEntities = templateEntityBlueprint->entityTemplates[i].exposedEntities;
				rootEntityTemplate->exposedInterfaces = templateEntityBlueprint->entityTemplates[i].exposedInterfaces;
				rootEntityTemplate->entitySubsets = templateEntityBlueprint->entityTemplates[i].entitySubsets;

				AddChildren(templateEntity, templateEntityBlueprint, rootEntityTemplate, resource);

				entityTemplate->rootEntityTemplate = rootEntityTemplate;

				break;
			}
		}

		entityTemplate->pinConnections = templateEntityBlueprint->pinConnections;
		entityTemplate->inputPinForwardings = templateEntityBlueprint->inputPinForwardings;
		entityTemplate->outputPinForwardings = templateEntityBlueprint->outputPinForwardings;

		//operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));
		//operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));

		return entityTemplate;
	}

	void ZEntityTemplate::AddChildren(IOI::STemplateEntity* templateEntity, IOI::STemplateEntityBlueprint* templateEntityBlueprint, SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const ZResource* resource)
	{
		for (unsigned i = 0; i < templateEntity->entityTemplates.Size(); ++i)
		{
			if (templateEntity->entityTemplates[i].parentIndex == subEntityTemplate->index)
			{
				SEntityTemplate::SSubEntityTemplate* subEntityTemplate2 = new SEntityTemplate::SSubEntityTemplate();
				int templateEntityResourceIndex = templateEntity->entityTemplates[i].entityTypeResourceIndex;
				int templateEntityBlueprintResourceIndex = templateEntityBlueprint->entityTemplates[i].entityTypeResourceIndex;

				std::string templateEntityResourceID = resource->GetReferences()->operator[](templateEntityResourceIndex)->GetResourceID();
				std::string templateEntityBlueprintResourceID = std::format("{}.pc_entityblueprint", templateEntityResourceID.substr(0, templateEntityResourceID.find_last_of('.')));

				subEntityTemplate2->templateEntityResourceID = templateEntityResourceID;
				subEntityTemplate2->templateEntityBlueprintResourceID = templateEntityBlueprintResourceID;
				subEntityTemplate2->index = i;
				subEntityTemplate2->parentIndex = templateEntity->entityTemplates[i].parentIndex;
				subEntityTemplate2->entityTypeResourceIndex = templateEntityResourceIndex;
				subEntityTemplate2->entityName = templateEntityBlueprint->entityTemplates[i].entityName;
				subEntityTemplate2->propertyValues = templateEntity->entityTemplates[i].propertyValues;
				subEntityTemplate2->postInitPropertyValues = templateEntity->entityTemplates[i].postInitPropertyValues;
				subEntityTemplate2->propertyAliases = templateEntityBlueprint->entityTemplates[i].propertyAliases;
				subEntityTemplate2->exposedEntities = templateEntityBlueprint->entityTemplates[i].exposedEntities;
				subEntityTemplate2->exposedInterfaces = templateEntityBlueprint->entityTemplates[i].exposedInterfaces;
				subEntityTemplate2->entitySubsets = templateEntityBlueprint->entityTemplates[i].entitySubsets;

				AddChildren(templateEntity, templateEntityBlueprint, subEntityTemplate2, resource);

				subEntityTemplate->children.PushBack(subEntityTemplate2);
			}
		}

		for (unsigned i = 0; i < subEntityTemplate->children.Size(); ++i)
		{
			for (unsigned j = i + 1; j < subEntityTemplate->children.Size(); ++j)
			{
				if (strcmp(subEntityTemplate->children[i]->entityName.ToCString(), subEntityTemplate->children[j]->entityName.ToCString()) > 0)
				{
					SEntityTemplate::SSubEntityTemplate* temp = subEntityTemplate->children[i];

					subEntityTemplate->children[i] = subEntityTemplate->children[j];
					subEntityTemplate->children[j] = temp;
				}
			}
		}
	}

	ZResource* ZEntityTemplate::GetTemplateEntityBlueprintReference(const ZResource* resource, int blueprintIndexInResourceHeader)
	{
		const std::vector<ZResource*>* references = resource->GetReferences();

		return references->operator[](blueprintIndexInResourceHeader);
	}

	MapNode* ZEntityTemplate::GetGeometryMap(const ZResource* resource, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary)
	{
		std::vector<MapNode*> geometryEntities;

		char buffer[MAX_PATH];

		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		std::string::size_type position = std::string(buffer).find_last_of("\\/");
		std::string mapFolder = std::format("{}/Map", std::string(buffer).substr(0, position));

		if (!std::filesystem::exists(mapFolder))
		{
			std::filesystem::create_directories(mapFolder);
		}

		for (unsigned i = 0; i < templateEntity->entityTemplates.Size(); ++i)
		{
			MapNode* childMapNode = new MapNode();

			childMapNode->index = i;
			childMapNode->entityTypeResourceIndex = templateEntity->entityTemplates[i].entityTypeResourceIndex;

			for (unsigned j = 0; j < templateEntity->entityTemplates[i].propertyValues.Size(); ++j)
			{
				IOI::SEntityTemplateProperty* entityTemplateProperty = &templateEntity->entityTemplates[i].propertyValues[j];
				std::string propertyName = propertyRegistry.GetPropertyNameFromHash(entityTemplateProperty->nPropertyID);

				if (propertyName == "m_mTransform")
				{
					childMapNode->hasTransform = true;
					childMapNode->transform = *static_cast<IOI::SMatrix43*>(entityTemplateProperty->value.GetData());

					if (childMapNode->name.length() == 0)
					{
						childMapNode->name = templateEntityBlueprint->entityTemplates[i].entityName.ToCString();
					}
				}
				else if (propertyName == "m_eidParent")
				{
					IOI::SEntityTemplateReference* entityTemplateReference = static_cast<IOI::SEntityTemplateReference*>(entityTemplateProperty->value.GetData());

					childMapNode->hasEIDParent = true;
					childMapNode->eidParent = entityTemplateReference->entityIndex;
				}
				else if (propertyName == "m_bVisible")
				{
					childMapNode->hasVisibleProperty = true;
					childMapNode->visible = *static_cast<bool*>(entityTemplateProperty->value.GetData());
				}
				else if (propertyName == "m_vGlobalSize")
				{
					childMapNode->hasGlobalSize = true;
					childMapNode->globalSize = *static_cast<IOI::SVector3*>(entityTemplateProperty->value.GetData());
				}
				else if (propertyName == "m_ResourceID")
				{
					childMapNode->hasResourceID = true;
					childMapNode->primRuntimeResourceID = *static_cast<IOI::ZRuntimeResourceID*>(entityTemplateProperty->value.GetData());
				}
			}

			for (unsigned j = 0; j < templateEntity->entityTemplates[i].postInitPropertyValues.Size(); ++j)
			{
				IOI::SEntityTemplateProperty* entityTemplateProperty = &templateEntity->entityTemplates[i].postInitPropertyValues[j];
				std::string propertyName = propertyRegistry.GetPropertyNameFromHash(entityTemplateProperty->nPropertyID);

				if (propertyName == "m_mTransform")
				{
					childMapNode->hasTransform = true;
					childMapNode->transform = *static_cast<IOI::SMatrix43*>(entityTemplateProperty->value.GetData());

					if (childMapNode->name.length() == 0)
					{
						childMapNode->name = templateEntityBlueprint->entityTemplates[i].entityName.ToCString();
					}
				}
				else if (propertyName == "m_eidParent")
				{
					IOI::SEntityTemplateReference* entityTemplateReference = static_cast<IOI::SEntityTemplateReference*>(entityTemplateProperty->value.GetData());

					childMapNode->hasEIDParent = true;
					childMapNode->eidParent = entityTemplateReference->entityIndex;
				}
				else if (propertyName == "m_bVisible")
				{
					childMapNode->hasVisibleProperty = true;
					childMapNode->visible = *static_cast<bool*>(entityTemplateProperty->value.GetData());
				}
				else if (propertyName == "m_vGlobalSize")
				{
					childMapNode->hasGlobalSize = true;
					childMapNode->globalSize = *static_cast<IOI::SVector3*>(entityTemplateProperty->value.GetData());
				}
				else if (propertyName == "m_ResourceID")
				{
					childMapNode->hasResourceID = true;
					childMapNode->primRuntimeResourceID = *static_cast<IOI::ZRuntimeResourceID*>(entityTemplateProperty->value.GetData());
				}
			}

			if (childMapNode->hasTransform)
			{
				ZResource* primReference;

				if (childMapNode->hasResourceID)
				{
					std::vector<ZResourceLibrary*>* resourceLibraries = headerLibrary->GetResourceLibraries();
					unsigned int resourceLibraryIndex = headerLibrary->GetIndexOfResourceLibrary(childMapNode->primRuntimeResourceID, false);
					unsigned int resourceIndex = headerLibrary->GetIndexOfResource(childMapNode->primRuntimeResourceID);

					primReference = resourceLibraries->operator[](resourceLibraryIndex)->GetResources()->operator[](resourceIndex);
				}
				else
				{
					ZResource* reference = resource->GetReferences()->operator[](childMapNode->entityTypeResourceIndex);
					primReference = GetPrimReference(reference, headerLibrary, globalHeaderLibrary);

					childMapNode->primReference = primReference;

					if (primReference)
					{
						//childMapNode->primRuntimeResourceID = *primReference->GetRuntimeResourceID();

						//std::string fileName = std::format("{}_{}.obj", childMapNode->name, ZUtility::ConvertValueToHexString(reference->GetRuntimeResourceID()->GetID()).substr(2));
						//std::string objFilePath = std::format("{}/Map/{}", std::string(buffer).substr(0, position), fileName);

						//ExportMapModel(primReference, objFilePath);

						/*std::string fileName = std::format("{}_{}.txt", childMapNode->name, ZUtility::ConvertValueToHexString(reference->GetRuntimeResourceID()->GetID()).substr(2));
						std::string textFilePath = std::format("{}/Map/{}", std::string(buffer).substr(0, position), fileName);

						std::ofstream file(textFilePath);

						if (file.is_open())
						{
							file << childMapNode->transform.Trans.x << ";" << childMapNode->transform.Trans.y << ";" << childMapNode->transform.Trans.z;

							if (childMapNode->hasPrimitiveScale)
							{
								file << std::endl << childMapNode->primitiveScale.x << ";" << childMapNode->primitiveScale.y << ";" << childMapNode->primitiveScale.z;
							}

							file.close();
						}*/
					}
				}

				geometryEntities.push_back(childMapNode);
			}
			else
			{
				delete childMapNode;
			}
		}

		MapNode* rootMapNode = new MapNode();

		rootMapNode->eidParent = -1;

		for (unsigned int i = 0; i < geometryEntities.size(); ++i)
		{
			if (geometryEntities[i]->eidParent == templateEntity->rootEntityIndex)
			{
				AddChildren(geometryEntities[i], geometryEntities);

				rootMapNode->children.push_back(geometryEntities[i]);
			}
		}

		/*#pragma omp parallel for
		for (int i = 0; i < geometryEntities.size(); ++i)
		{
			ZResource* reference = resource->GetReferences()->operator[](geometryEntities[i]->entityTypeResourceIndex);
			ZResource* primReference = GetPrimReference(reference, headerLibrary, globalHeaderLibrary);

			if (primReference)
			{
				geometryEntities[i]->primRuntimeResourceID = *primReference->GetRuntimeResourceID();

				std::string fileName = std::format("{}_{}.obj", geometryEntities[i]->name, ZUtility::ConvertValueToHexString(reference->GetRuntimeResourceID()->GetID()).substr(2));
				std::string objFilePath = std::format("{}/Map/{}", std::string(buffer).substr(0, position), fileName);

				ExportMapModel(primReference, headerLibrary, globalHeaderLibrary, objFilePath);
			}

			int thread = omp_get_thread_num();
			std::string s = "s";
		}*/

		//#pragma omp parallel for
		//for (int i = 0; i < geometryEntities.size(); ++i)
		//{
		//	if (geometryEntities[i]->primReference)
		//	{
		//		std::string fileName = std::format("{}_{}.obj", geometryEntities[i]->name, ZUtility::ConvertValueToHexString(geometryEntities[i]->primReference->GetRuntimeResourceID()->GetID()).substr(2));
		//		std::string objFilePath = std::format("{}/Map/{}", std::string(buffer).substr(0, position), fileName);

		//		ExportMapModel(geometryEntities[i]->primReference, objFilePath);
		//	}

		//	//int thread = omp_get_thread_num();
		//	//std::string s = "s";
		//}

		/*std::for_each(
			std::execution::par,
			geometryEntities.begin(),
			geometryEntities.end(),
			[](auto&& item)
			{

			});*/

		return rootMapNode;
	}

	void ZEntityTemplate::AddChildren(MapNode* parentNode, std::vector<MapNode*>& geometryEntities)
	{
		for (unsigned i = 0; i < geometryEntities.size(); ++i)
		{
			if (parentNode->index == i)
			{
				continue;
			}

			if (geometryEntities[i]->eidParent == parentNode->index)
			{
				AddChildren(geometryEntities[i], geometryEntities);

				parentNode->children.push_back(geometryEntities[i]);
			}
		}
	}

	ZResource* ZEntityTemplate::GetPrimReference(ZResource* reference, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary)
	{
		if (!reference->IsResourceDataLoaded())
		{
			std::vector<ZResourceLibrary*>* resourceLibraries = headerLibrary->GetResourceLibraries();
			unsigned int resourceLibraryIndex = headerLibrary->GetIndexOfResourceLibrary(*reference->GetRuntimeResourceID(), false);
			unsigned int resourceIndex = headerLibrary->GetIndexOfResource(*reference->GetRuntimeResourceID());

			//resourceLibraries->operator[](resourceLibraryIndex)->LoadResourceData(resourceIndex);
			headerLibrary->LoadResourceReferences(resourceLibraryIndex, resourceIndex, globalHeaderLibrary);
		}

		ZResource* result = {};

		if (reference->GetResourceHeaderHeader()->m_type == 0x41534554) //ASET
		{
			const std::vector<ZResource*>* references = reference->GetReferences();

			for (unsigned i = 0; i < references->size(); ++i)
			{
				ZResource* reference2 = references->operator[](i);

				if (reference2->GetResourceHeaderHeader()->m_type == 0x54454D50) //TEMP
				{
					if (!reference2->IsResourceDataLoaded())
					{
						std::vector<ZResourceLibrary*>* resourceLibraries = headerLibrary->GetResourceLibraries();
						unsigned int resourceLibraryIndex = headerLibrary->GetIndexOfResourceLibrary(*reference2->GetRuntimeResourceID(), false);
						unsigned int resourceIndex = headerLibrary->GetIndexOfResource(*reference2->GetRuntimeResourceID());

						//resourceLibraries->operator[](resourceLibraryIndex)->LoadResourceData(resourceIndex);
						headerLibrary->LoadResourceReferences(resourceLibraryIndex, resourceIndex, globalHeaderLibrary);
					}

					HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(reference2->GetResourceLibraryPath());

					resourceLibraryBinaryReader.GetInputStream()->Seek(reference2->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					unsigned int dataSize = reference2->GetResourceDataSize();
					void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

					reference2->SetResourceData(data);

					IOI::ZRuntimeResourceID runtimeResourceID = GetPrimRuntimeResourceID(reference2);

					delete[] data;

					if (runtimeResourceID.GetID() != 0)
					{
						const std::vector<ZResource*>* references2 = reference2->GetReferences();

						for (unsigned j = 0; j < references2->size(); ++j)
						{
							if (*references2->operator[](j)->GetRuntimeResourceID() == runtimeResourceID)
							{
								result = references2->operator[](j);

								break;
							}
						}

						break;
					}
				}
			}
		}
		else if (reference->GetResourceHeaderHeader()->m_type == 0x54454D50)
		{
			HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(reference->GetResourceLibraryPath());

			resourceLibraryBinaryReader.GetInputStream()->Seek(reference->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

			unsigned int dataSize = reference->GetResourceDataSize();
			void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

			reference->SetResourceData(data);

			IOI::ZRuntimeResourceID runtimeResourceID = GetPrimRuntimeResourceID(reference);

			delete[] data;

			const std::vector<ZResource*>* references = reference->GetReferences();

			for (unsigned j = 0; j < references->size(); ++j)
			{
				if (*references->operator[](j)->GetRuntimeResourceID() == runtimeResourceID)
				{
					result = references->operator[](j);

					break;
				}
			}
		}

		return result;
	}

	IOI::ZRuntimeResourceID ZEntityTemplate::GetPrimRuntimeResourceID(const ZResource* reference)
	{
		IOI::ZRuntimeResourceID result = {};
		static IOI::ZBinaryDeserializer binaryDeserializer;
		bool found = false;

		const void* data = reference->GetResourceData();
		const unsigned int dataSize = reference->GetResourceDataSize();
		IOI::STemplateEntity* templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(data, dataSize, reference->GetReferences()));

		for (unsigned i = 0; i < templateEntity->entityTemplates.Size(); ++i)
		{
			IOI::STemplateSubEntity* templateSubEntity = &templateEntity->entityTemplates[i];

			for (unsigned int j = 0; j < templateSubEntity->propertyValues.Size(); ++j)
			{
				std::string propertyName = propertyRegistry.GetPropertyNameFromHash(templateSubEntity->propertyValues[j].nPropertyID);

				if (propertyName == "m_ResourceID")
				{
					result = *static_cast<IOI::ZRuntimeResourceID*>(templateSubEntity->propertyValues[j].value.GetData());
					found = true;

					break;
				}
			}

			if (!found)
			{
				for (unsigned int j = 0; j < templateSubEntity->postInitPropertyValues.Size(); ++j)
				{
					std::string propertyName = propertyRegistry.GetPropertyNameFromHash(templateSubEntity->postInitPropertyValues[j].nPropertyID);

					if (propertyName == "m_ResourceID")
					{
						result = *static_cast<IOI::ZRuntimeResourceID*>(templateSubEntity->postInitPropertyValues[j].value.GetData());

						break;
					}
				}
			}
		}

		operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));

		return result;
	}

	void ZEntityTemplate::ExportMapModel(ZResource* reference, const std::string& objFilePath)
	{
		HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(reference->GetResourceLibraryPath());

		resourceLibraryBinaryReader.GetInputStream()->Seek(reference->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		unsigned int dataSize = reference->GetResourceDataSize();
		void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

		ZModel::ConvertRenderPrimitiveToOBJ(data, dataSize, objFilePath);

		delete[] data;
	}
}
