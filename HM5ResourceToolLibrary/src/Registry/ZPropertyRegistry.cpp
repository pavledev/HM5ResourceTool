#include "pch.h"
#include "Registry/ZPropertyRegistry.hpp"

void ZPropertyRegistry::DeserializeFromJSON(rapidjson::Document& document, ZPropertyRegistry& propertyRegistry)
{
	const rapidjson::Value& properties = document["properties"];

	for (rapidjson::Value::ConstValueIterator it = properties.Begin(); it != properties.End(); ++it)
	{
		const rapidjson::Value& object = it->GetObj();
		std::string name = object["name"].GetString();
		unsigned int hash = object["hash"].GetUint();

		propertyRegistry.properties.insert(std::make_pair(hash, name));
	}
}

std::string ZPropertyRegistry::GetPropertyNameFromHash(const unsigned int hash)
{
	std::string result;
	auto it = properties.find(hash);

	if (it != properties.end())
	{
		result = it->second;
	}

	return result;
}

bool ZPropertyRegistry::IsEmpty()
{
	return properties.empty();
}
