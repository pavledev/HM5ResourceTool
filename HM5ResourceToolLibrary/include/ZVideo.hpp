#pragma once

#include <string>
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"

namespace HM5ResourceTool
{
    class __declspec(dllexport) ZVideo
    {
    public:
        static bool ConvertPCBinkVidToBINK(const std::string& pcBinkVidPath, const std::string& outputFilePath);
        static bool ConvertPCBinkVidToBINK(const std::string& pcBinkVidPath, void** binkData, unsigned int& binkSize);
        static bool ConvertBINKToPCBinkVid(const std::string& binkPath);
    };
}
