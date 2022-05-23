#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"

using namespace rapidjson;

namespace IOI
{
	class ZBinarySerializer;

	class ZRuntimeResourceID
	{
	private:
		unsigned int m_IDHigh;
		unsigned int m_IDLow;

	public:
		ZRuntimeResourceID() = default;
		unsigned long long GetID() const;
		__declspec(dllexport) unsigned long long GetID();
		static ZRuntimeResourceID QueryRuntimeResourceID(const std::string& idResource);
		static std::string QueryResourceID(const ZRuntimeResourceID& ridResource);
		__declspec(dllexport) static ZRuntimeResourceID Create(unsigned long long nResource);
		static ZRuntimeResourceID CreateLibraryResourceID(ZRuntimeResourceID ridLibrary, int indexInLibrary);
		__declspec(dllexport) bool IsLibraryResource() const;
		bool IsLibrary() const;
		ZRuntimeResourceID GetLibraryRuntimeResourceID() const;
		int GetIndexInLibrary() const;
		__declspec(dllexport) int GetIndexInLibrary();
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static ZRuntimeResourceID* DeserializeFromJSON(const rapidjson::Value& object);
		static unsigned long long CreateMD5(const std::string& resourceID);
		__declspec(dllexport) unsigned int GetHigh();
		__declspec(dllexport) unsigned int GetLow();
		const unsigned int GetLow() const;
		__declspec(dllexport) void SetHigh(unsigned int idHigh);
		__declspec(dllexport) void SetLow(unsigned int idLow);
		bool operator==(const ZRuntimeResourceID& other);
	};
}
