#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <string>
#include "Hash/MD5.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"

namespace IOI
{
    class ZLocalResourceFileMediator
    {
    public:
        static std::string CreateMD5(std::string resourceID);
        static std::string CalcResourceFileName(std::string resourceID);
        static std::string GetResourceFileName(const ZRuntimeResourceID& resourceID);
    };
}
