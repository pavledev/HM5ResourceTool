#include "pch.h"
#include "Glacier2/Entity/SEntityTemplatePinConnection.hpp"

namespace IOI
{
    void SEntityTemplatePinConnection::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("fromID");
        writer.Int(fromID);

        writer.String("toID");
        writer.Int(toID);

        writer.String("fromPinName");
        fromPinName.SerializeToJSON(writer);

        writer.String("toPinName");
        toPinName.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SEntityTemplatePinConnection::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int fromIDOffset = offset + offsetof(SEntityTemplatePinConnection, fromID);
        unsigned int toIDOffset = offset + offsetof(SEntityTemplatePinConnection, toID);
        unsigned int fromPinNameOffset = offset + offsetof(SEntityTemplatePinConnection, fromPinName);
        unsigned int toPinNameOffset = offset + offsetof(SEntityTemplatePinConnection, toPinName);

        binarySerializer.WriteToMemory(&fromID, sizeof(int), fromIDOffset);
        binarySerializer.WriteToMemory(&toID, sizeof(int), toIDOffset);

        fromPinName.SerializeToMemory(binarySerializer, fromPinNameOffset);
        toPinName.SerializeToMemory(binarySerializer, toPinNameOffset);
    }

    SEntityTemplatePinConnection* SEntityTemplatePinConnection::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SEntityTemplatePinConnection* entityTemplatePinConnection = new SEntityTemplatePinConnection();

        entityTemplatePinConnection->fromID = object["fromID"].GetInt();
        entityTemplatePinConnection->toID = object["toID"].GetInt();
        entityTemplatePinConnection->fromPinName = *ZString::DeserializeFromJSON(object["fromPinName"].GetString());

        return entityTemplatePinConnection;
    }
}
