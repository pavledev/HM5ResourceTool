#pragma once

#include <vector>
#include <map>
#include "Glacier2/TArray.hpp"
#include "Glacier2/TPair.hpp"
#include "Glacier2/Entity/SEntityTemplateProperty.hpp"
#include "Glacier2/Entity/SEntityTemplatePropertyAlias.hpp"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/Entity/SEntityTemplateEntitySubset.hpp"
#include "Glacier2/Entity/SEntityTemplatePinConnection.hpp"

namespace IOI
{
	class ZBinarySerializer;
}

namespace HM5ResourceTool
{
	struct SEntityTemplate
	{
		struct SSubEntityTemplate
		{
			std::string templateEntityResourceID;
			std::string templateEntityBlueprintResourceID;
			int index;
			int parentIndex;
			int entityTypeResourceIndex;
			IOI::ZString entityName;
			IOI::TArray<IOI::SEntityTemplateProperty> propertyValues;
			IOI::TArray<IOI::SEntityTemplateProperty> postInitPropertyValues;
			IOI::TArray<IOI::SEntityTemplatePropertyAlias> propertyAliases;
			IOI::TArray<IOI::TPair<IOI::ZString, IOI::SEntityTemplateReference>> exposedEntities;
			IOI::TArray<IOI::TPair<IOI::ZString, int>> exposedInterfaces;
			IOI::TArray<IOI::TPair<IOI::ZString, IOI::SEntityTemplateEntitySubset>> entitySubsets;
			IOI::TArray<SSubEntityTemplate*> children;

			void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
			std::map<std::string, std::map<std::string, std::vector<std::string>>> GetPropertyTypesAndValues();
			std::map<std::string, std::map<std::string, std::vector<std::string>>> GetPostInitPropertyTypeAndValues();
			std::map<std::string, std::vector<std::string>> GetPropertyTypeAndValues(IOI::SEntityTemplateProperty* entityTemplateProperty);
			std::vector<std::string> GetPropertyAliases();
			std::map <std::string, std::string> GetExposedEntities();
		};

		int blueprintIndexInResourceHeader;
		int rootEntityIndex;
		SSubEntityTemplate* rootEntityTemplate;
		IOI::TArray<IOI::SEntityTemplatePinConnection> pinConnections;
		IOI::TArray<IOI::SEntityTemplatePinConnection> inputPinForwardings;
		IOI::TArray<IOI::SEntityTemplatePinConnection> outputPinForwardings;
		inline static ZPropertyRegistry propertyRegistry;
		inline static ZEnumRegistry enumRegistry;

		SEntityTemplate();
		//std::string SerializeToJSON();
		//void SerializeToMemory(IOI::ZBinarySerializer& binarySerializer);
		//static SEntityTemplate* DeserializeFromJSON(const rapidjson::Document& document);
		__declspec(dllexport) std::string GetEntityDetails(int index);
		__declspec(dllexport) std::string GetEntityName(int index);
		__declspec(dllexport) std::string GetTemplateEntityResourceID(int index);
		__declspec(dllexport) std::string GetTemplateEntityBlueprintResourceID(int index);
		SEntityTemplate::SSubEntityTemplate* FindEntity(int index);
		SEntityTemplate::SSubEntityTemplate* FindEntity(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, int index);
		void SerializePinsToJSON(SEntityTemplate::SSubEntityTemplate* entity, const IOI::TArray<IOI::SEntityTemplatePinConnection>& pins, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		__declspec(dllexport) std::set<int> SearchContent(const std::string& content, const bool exactMatch);
		void SearchContent(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const std::string& content, const bool exactMatch, std::set<int>& indices);
		bool SearchContent(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const std::string& content, const bool exactMatch);
		static void LoadEnumRegistry();
		__declspec(dllexport) std::map<std::string, std::map<std::string, std::vector<std::string>>> GetPropertyTypesAndValues(int index);
		__declspec(dllexport) std::map<std::string, std::map<std::string, std::vector<std::string>>> GetPostInitPropertyTypesAndValues(int index);
		__declspec(dllexport) std::vector<std::string> GetEntityReferences(int index);
		void GetPropertyReferences(IOI::TArray<IOI::SEntityTemplateProperty>* properties, std::vector<std::string>& references);
		void GetPinReferences(int entityIndex, std::vector<std::string>& references);
		__declspec(dllexport) std::set<std::string> GetEntityBackReferences(int index);
		void GetEntityBackReferences(int index, SEntityTemplate::SSubEntityTemplate* entity, std::set<std::string>& backReferences);
		void GetPropertyBackReferences(int index, IOI::TArray<IOI::SEntityTemplateProperty>* properties, std::set<std::string>& backReferences);
		void GetPinBackReferences(int entityIndex, std::set<std::string>& backReferences);
		__declspec(dllexport) std::vector<std::string> GetPropertyAliases(int index);
		__declspec(dllexport) std::map <std::string, std::string> GetExposedEntities(int index);
	};
}
