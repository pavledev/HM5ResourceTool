#include "pch.h"
#include "Glacier2/PackageList/SPackageListData.hpp"

namespace IOI
{
    std::string SPackageListData::SerializeToJSON()
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
