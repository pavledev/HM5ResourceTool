#pragma once

#include <cstdint>
#include <string.h>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
	class ZBinarySerializer;

	class ZString
	{
	public:
		__declspec(dllexport) ZString();
		ZString(const char* rhs);
		ZString(const ZString& other);
		__declspec(dllexport) ZString& operator=(const ZString& other);
		__declspec(dllexport) ~ZString();
		const unsigned int Length() const;
		__declspec(dllexport) const char* ToCString() const;
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static ZString* DeserializeFromJSON(const char* chars);
		bool operator==(const ZString& other);

	private:
		unsigned int m_length;
		const char* m_chars;
	};
}
