#include "pch.h"
#include "Glacier2/SResourceIndex.hpp"

namespace IOI
{
    std::string SResourceIndex::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("folders");
        folders.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }
}
