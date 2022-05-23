#include "pch.h"
#include "Glacier2/PackageList/SPackageListDataSection.hpp"

namespace IOI
{
    void SPackageListDataSection::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("sceneResourceIDs");
        sceneResourceIDs.SerializeToJSON(writer);

        writer.String("resources");
        resources.SerializeToJSON(writer);

        writer.EndObject();
    }
}
