#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "ZObjectRef.hpp"
#include "IO/ZJSONSerializer.hpp"

using namespace rapidjson;

namespace IOI
{
	class ZBinarySerializer;

	class ZVariant : public ZObjectRef
	{
	public:
		ZVariant();
		//~ZVariant();
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		bool SerializeData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		static ZVariant* DeserializeFromJSON(const rapidjson::Value& object);
		static void* GetValue(const ZVariant* variant, const std::unordered_map<IOI::STypeID*, std::vector<void*>>& values);
		static void LoadEnumRegistry();

	private:
		inline static ZEnumRegistry enumRegistry;
	};
}
