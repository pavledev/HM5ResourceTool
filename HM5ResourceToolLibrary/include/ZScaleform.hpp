#pragma once

#include <string>
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDS.h"
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "Glacier2/Scaleform/SGfxResource.hpp"
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "Scaleform/SWFRender.hpp"
#include "Scaleform/PNG.hpp"

namespace HM5ResourceTool
{
    class ZScaleform
    {
    public:
        static bool Convert(const std::string& swffPath, const std::string& outputFilePath);
        static bool Convert(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        static void Convert(IO::ZBinaryReader& binaryReader, const std::string& outputFilePath);
        static bool ConvertSWFFToPNG(const void* data, const unsigned int dataSize, const std::string& outputFilePath, bool isSwf = false);
        static bool ConvertGFXFToGFX(const std::string& gfxfPath, const std::string& outputFilePath);
        static bool ConvertGFXFToGFX(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        static void ConvertGFXFToGFX(IOI::SGfxResource* gfxResource, const std::string& outputFilePath, unsigned char alignment);
        static bool ConvertDDSToSWFF(const std::string& ddsPath);
        static bool ConvertPNGToSWFF(const std::string& pngPath);
        static bool ConvertSWFToSWFF(const std::string& swfPath);
        static bool ConvertToSWFF(IO::ZBinaryReader& inputBinaryReader, IO::ZBinaryReader& swffBinaryReader, const std::string& inputFilePath);
        static std::string GetErrorMessage(HRESULT hresult);
    };
}
