#pragma once

#include <unordered_map>
#include <map>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

class ZEnumRegistry
{
public:
	static void DeserializeFromJSON(rapidjson::Document& document, ZEnumRegistry& enumRegistry);
	std::map<std::string, int> GetEnumByTypeName(const std::string& typeName);
	bool IsEmpty();

private:
	std::unordered_map<std::string, std::map<std::string, int>> enums;
};

