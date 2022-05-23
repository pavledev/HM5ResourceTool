#include "pch.h"
#include "Glacier2/PackageList/SPackageListDataFolder.hpp"

namespace IOI
{
    void SPackageListDataFolder::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("name");
        name.SerializeToJSON(writer);

        writer.String("sections");
        sections.SerializeToJSON(writer);

        writer.EndObject();
    }
}
