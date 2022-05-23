#include "pch.h"
#include "Glacier2/Entity/SCppEntitySubsetInfo.hpp"

namespace IOI
{
    void SCppEntitySubsetInfo::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("name");
        name.SerializeToJSON(writer);

        writer.String("type");
        type->SerializeToJSON(writer);

        writer.String("flags");
        writer.Uint(flags);

        writer.EndObject();
    }

    void SCppEntitySubsetInfo::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int nameOffset = offset + offsetof(SCppEntitySubsetInfo, name);
        unsigned int typeOffset = offset + offsetof(SCppEntitySubsetInfo, type);
        unsigned int flagsOffset = offset + offsetof(SCppEntitySubsetInfo, flags);

        name.SerializeToMemory(binarySerializer, nameOffset);
        type->SerializeToMemory(binarySerializer, typeOffset);

        binarySerializer.WriteToMemory(&flags, sizeof(unsigned int), flagsOffset);
    }

    SCppEntitySubsetInfo* SCppEntitySubsetInfo::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SCppEntitySubsetInfo* cppEntitySubsetInfo = new SCppEntitySubsetInfo();

        cppEntitySubsetInfo->name = *ZString::DeserializeFromJSON(object["name"].GetString());
        cppEntitySubsetInfo->type = STypeID::DeserializeFromJSON(object, "type");
        cppEntitySubsetInfo->flags = object["flags"].GetUint();

        return cppEntitySubsetInfo;
    }
}
