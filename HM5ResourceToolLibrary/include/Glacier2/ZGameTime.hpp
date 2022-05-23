#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
	class ZBinarySerializer;

	class ZGameTime
	{
	public:
		long long GetTicks() const;
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static ZGameTime* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const ZGameTime& other);

	private:
		long long m_nTicks;
	};
}
