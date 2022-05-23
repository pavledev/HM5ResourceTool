#include "pch.h"
#include "Registry/ZEnumRegistry.hpp"

void ZEnumRegistry::DeserializeFromJSON(rapidjson::Document& document, ZEnumRegistry& enumRegistry)
{
	const rapidjson::Value& enums = document["enums"];

	for (rapidjson::Value::ConstValueIterator it = enums.Begin(); it != enums.End(); ++it)
	{
		const rapidjson::Value& object = it->GetObj();
		const rapidjson::Value& items = object["items"];
		std::map<std::string, int> items2;

		for (rapidjson::Value::ConstValueIterator it2 = items.Begin(); it2 != items.End(); ++it2)
		{
			const rapidjson::Value& object2 = it2->GetObj();

			items2.insert(std::make_pair(object2["name"].GetString(), object2["value"].GetInt()));
		}

		enumRegistry.enums.insert(std::make_pair(object["name"].GetString(), items2));
	}
}

std::map<std::string, int> ZEnumRegistry::GetEnumByTypeName(const std::string& typeName)
{
	std::map<std::string, int> result;
	auto it = enums.find(typeName);

	if (it != enums.end())
	{
		result = it->second;
	}

	return result;
}

bool ZEnumRegistry::IsEmpty()
{
	return enums.empty();
}
