#pragma once

#include <unordered_map>
#include <map>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

class ZPropertyRegistry
{
public:
	static void DeserializeFromJSON(rapidjson::Document& document, ZPropertyRegistry& propertyRegistry);
	std::string GetPropertyNameFromHash(const unsigned int hash);
	bool IsEmpty();

private:
	std::unordered_map<unsigned int, std::string> properties;
};
