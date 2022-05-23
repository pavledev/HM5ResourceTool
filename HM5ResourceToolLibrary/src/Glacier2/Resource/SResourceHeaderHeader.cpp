#include "pch.h"
#include "Glacier2/Resource/SResourceHeaderHeader.hpp"
#include "ZUtility.hpp"

namespace IOI
{
	void SResourceHeaderHeader::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();

		writer.String("m_type");
		writer.String(ZUtility::ConvertTypeToString(m_type).c_str());

		writer.String("m_nReferencesChunkSize");
		writer.String(ZUtility::ConvertValueToHexString(m_nReferencesChunkSize).c_str());

		writer.String("m_nStatesChunkSize");
		writer.String(ZUtility::ConvertValueToHexString(m_nStatesChunkSize).c_str());

		writer.String("m_nDataSize");
		writer.String(ZUtility::ConvertValueToHexString(m_nDataSize).c_str());

		writer.String("m_nSystemMemoryRequirement");
		writer.String(ZUtility::ConvertValueToHexString(m_nSystemMemoryRequirement).c_str());

		writer.String("m_nVideoMemoryRequirement");
		writer.String(ZUtility::ConvertValueToHexString(m_nVideoMemoryRequirement).c_str());

		writer.EndObject();
	}
}
