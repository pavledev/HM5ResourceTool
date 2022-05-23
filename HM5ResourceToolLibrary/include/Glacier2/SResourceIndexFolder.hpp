#pragma once

#include "ZString.hpp"
#include "TArray.hpp"

namespace IOI
{
    struct SResourceIndexFolder
    {
        ZString name;
        TArray<int> resourceIndices;

        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
    };
}
