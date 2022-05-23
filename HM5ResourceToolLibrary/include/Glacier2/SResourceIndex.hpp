#pragma once

#include "SResourceIndexFolder.hpp"

namespace IOI
{
    struct SResourceIndex
    {
        TArray<SResourceIndexFolder> folders;

        std::string SerializeToJSON();
    };
}
