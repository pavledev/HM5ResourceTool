#include "pch.h"
#include "Glacier2/ZGameTime.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
    long long ZGameTime::GetTicks() const
    {
        return m_nTicks;
    }

    void ZGameTime::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nTicks");
        writer.Int64(m_nTicks);

        writer.EndObject();
    }

    void ZGameTime::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int ticksOffset = offset + offsetof(ZGameTime, m_nTicks);

        binarySerializer.WriteToMemory(&m_nTicks, sizeof(long long), ticksOffset);
    }

    ZGameTime* ZGameTime::DeserializeFromJSON(const rapidjson::Value& object)
    {
        ZGameTime* gameTime = new ZGameTime();

        gameTime->m_nTicks = object["m_nTicks"].GetInt64();

        return gameTime;
    }

    bool ZGameTime::operator==(const ZGameTime& other)
    {
        return m_nTicks == other.m_nTicks;
    }
}
