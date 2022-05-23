#pragma once

#include <unordered_map>
#include <map>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "Glacier2/TypeInfo/STypeID.hpp"

class ZTypeRegistry
{
public:
	static void DeserializeFromJSON(rapidjson::Document& document, ZTypeRegistry& typeRegistry);
	IOI::STypeID* GetTypeIDFromTypeName(const std::string& typeName);
	bool IsEmpty();

private:
	std::unordered_map<std::string, IOI::STypeID*> typeIDs;
};
