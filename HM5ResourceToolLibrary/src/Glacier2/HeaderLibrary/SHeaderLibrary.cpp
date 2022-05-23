#include "pch.h"
#include "Glacier2/HeaderLibrary/SHeaderLibrary.hpp"

namespace IOI
{
    std::string SHeaderLibrary::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("chunks");
        chunks.SerializeToJSON(writer);

        writer.String("externalResourceIds");
        externalResourceIds.SerializeToJSON(writer);

        writer.String("ridSource");
        writer.Int64(ridSource);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    void SHeaderLibrary::SerializeToMemory(ZBinarySerializer& binarySerializer)
    {
        unsigned int chunksOffset = offsetof(SHeaderLibrary, chunks);
        unsigned int externalResourceIdsOffset = offsetof(SHeaderLibrary, externalResourceIds);
        unsigned int ridSourceOffset = offsetof(SHeaderLibrary, ridSource);

        binarySerializer.WriteToMemory(&ridSource, sizeof(long long), ridSourceOffset);

        chunks.SerializeToMemory(binarySerializer, chunksOffset);
        externalResourceIds.SerializeToMemory(binarySerializer, externalResourceIdsOffset);
    }
}
