#include "pch.h"
#include "SEntityTemplate.hpp"

namespace HM5ResourceToolCLI
{
	SEntityTemplate::SSubEntityTemplate::SSubEntityTemplate(HM5ResourceTool::SEntityTemplate::SSubEntityTemplate* subEntityTemplate)
	{
		this->subEntityTemplate = subEntityTemplate;
	}

	int SEntityTemplate::SSubEntityTemplate::GetIndex()
	{
		return subEntityTemplate->index;
	}

	String^ SEntityTemplate::SSubEntityTemplate::GetEntityName()
	{
		return marshal_as<String^>(subEntityTemplate->entityName.ToCString());
	}

	List<SEntityTemplate::SSubEntityTemplate^>^ SEntityTemplate::SSubEntityTemplate::GetChildren()
	{
		List<SEntityTemplate::SSubEntityTemplate^>^ result = gcnew List<SEntityTemplate::SSubEntityTemplate^>();

		for (unsigned int i = 0; i < subEntityTemplate->children.Size(); ++i)
		{
			SEntityTemplate::SSubEntityTemplate^ subEntityTemplate2 = gcnew SEntityTemplate::SSubEntityTemplate(subEntityTemplate->children[i]);

			result->Add(subEntityTemplate2);
		}

		return result;
	}

	SEntityTemplate::SEntityTemplate(HM5ResourceTool::SEntityTemplate* entityTemplate) : ManagedObject(entityTemplate)
	{

	}

	SEntityTemplate::SSubEntityTemplate^ SEntityTemplate::GetRootEntityTemplate()
	{
		return gcnew SEntityTemplate::SSubEntityTemplate(instance->rootEntityTemplate);
	}

	String^ SEntityTemplate::GetEntityDetails(int index)
	{
		return marshal_as<String^>(instance->GetEntityDetails(index));
	}

	String^ SEntityTemplate::GetEntityName(int index)
	{
		return marshal_as<String^>(instance->GetEntityName(index));
	}

	String^ SEntityTemplate::GetTemplateEntityResourceID(int index)
	{
		return marshal_as<String^>(instance->GetTemplateEntityResourceID(index));
	}

	String^ SEntityTemplate::GetTemplateEntityBlueprintResourceID(int index)
	{
		return marshal_as<String^>(instance->GetTemplateEntityBlueprintResourceID(index));
	}

	List<int>^ SEntityTemplate::SearchContent(String^ content, bool exactMatch)
	{
		std::string content2 = marshal_as<std::string>(content);

		List<int>^ result = gcnew List<int>();
		std::set<int> indices = instance->SearchContent(content2, exactMatch);

		for (auto it = indices.begin(); it != indices.end(); ++it)
		{
			result->Add(*it);
		}

		return result;
	}

	Dictionary<String^, Dictionary<String^, List<String^>^>^>^ SEntityTemplate::GetPropertyTypesAndValues(int index)
	{
		std::map<std::string, std::map<std::string, std::vector<std::string>>> properties = instance->GetPropertyTypesAndValues(index);
		Dictionary<String^, Dictionary<String^, List<String^>^>^>^ result = gcnew Dictionary<String^, Dictionary<String^, List<String^>^>^>();

		for (auto it = properties.begin(); it != properties.end(); ++it)
		{
			std::map<std::string, std::vector<std::string>> typeNames = it->second;
			Dictionary<String^, List<String^>^>^ typeNames2 = gcnew Dictionary<String^, List<String^>^>();

			for (auto it2 = typeNames.begin(); it2 != typeNames.end(); ++it2)
			{
				std::vector<std::string> values = it2->second;
				List<String^>^ values2 = gcnew List<String^>();

				for (unsigned int i = 0; i < values.size(); ++i)
				{
					values2->Add(marshal_as<String^>(values[i]));
				}

				typeNames2->Add(marshal_as<String^>(it2->first), values2);
			}

			result->Add(marshal_as<String^>(it->first), typeNames2);
		}

		return result;
	}

	Dictionary<String^, Dictionary<String^, List<String^>^>^>^ SEntityTemplate::GetPostInitPropertyTypesAndValues(int index)
	{
		std::map<std::string, std::map<std::string, std::vector<std::string>>> properties = instance->GetPostInitPropertyTypesAndValues(index);
		Dictionary<String^, Dictionary<String^, List<String^>^>^>^ result = gcnew Dictionary<String^, Dictionary<String^, List<String^>^>^>();

		for (auto it = properties.begin(); it != properties.end(); ++it)
		{
			std::map<std::string, std::vector<std::string>> typeNames = it->second;
			Dictionary<String^, List<String^>^>^ typeNames2 = gcnew Dictionary<String^, List<String^>^>();

			for (auto it2 = typeNames.begin(); it2 != typeNames.end(); ++it2)
			{
				std::vector<std::string> values = it2->second;
				List<String^>^ values2 = gcnew List<String^>();

				for (unsigned int i = 0; i < values.size(); ++i)
				{
					values2->Add(marshal_as<String^>(values[i]));
				}

				typeNames2->Add(marshal_as<String^>(it2->first), values2);
			}

			result->Add(marshal_as<String^>(it->first), typeNames2);
		}

		return result;
	}

	List<String^>^ SEntityTemplate::GetEntityReferences(int index)
	{
		std::vector<std::string> references = instance->GetEntityReferences(index);
		List<String^>^ result = gcnew List<String^>();

		for (unsigned int i = 0; i < references.size(); ++i)
		{
			result->Add(marshal_as<String^>(references[i]));
		}

		return result;
	}

	List<String^>^ SEntityTemplate::GetEntityBackReferences(int index)
	{
		std::set<std::string> backReferences = instance->GetEntityBackReferences(index);
		List<String^>^ result = gcnew List<String^>();

		for (auto it = backReferences.begin(); it != backReferences.end(); ++it)
		{
			result->Add(marshal_as<String^>(*it));
		}

		return result;
	}

	List<String^>^ SEntityTemplate::GetPropertyAliases(int index)
	{
		std::vector<std::string> aliases = instance->GetPropertyAliases(index);
		List<String^>^ result = gcnew List<String^>();

		for (unsigned int i = 0; i < aliases.size(); ++i)
		{
			result->Add(marshal_as<String^>(aliases[i]));
		}

		return result;
	}
}
