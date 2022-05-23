#include "pch.h"
#include "Glacier2/SSettingsParamMultiplier.hpp"
#include "Glacier2/ZBinaryDeserializer.hpp"

namespace IOI
{
	void SSettingsParamMultiplier::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("r");
		writer.Uint(m_eSettingsParam);

		writer.String("g");
		writer.Double(m_fMultiplier);

		writer.EndObject();
	}

	void SSettingsParamMultiplier::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
	{
		unsigned int settingsParamOffset = offset + offsetof(SSettingsParamMultiplier, m_eSettingsParam);
		unsigned int multiplierOffset = offset + offsetof(SSettingsParamMultiplier, m_fMultiplier);

		binarySerializer.WriteToMemory(&m_eSettingsParam, sizeof(unsigned int), settingsParamOffset);
		binarySerializer.WriteToMemory(&m_fMultiplier, sizeof(float), multiplierOffset);
	}

	SSettingsParamMultiplier* SSettingsParamMultiplier::DeserializeFromJSON(const rapidjson::Value& object)
	{
		SSettingsParamMultiplier* settingsParamMultiplier = new SSettingsParamMultiplier();

		settingsParamMultiplier->m_eSettingsParam = static_cast<ESettingsParam>(object["m_eSettingsParam"].GetUint());
		settingsParamMultiplier->m_fMultiplier = object["m_fMultiplier"].GetFloat();

		return settingsParamMultiplier;
	}
}
