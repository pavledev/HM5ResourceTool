#pragma once

#include <string>
#include <vector>
#include "FSB/ZContainer.hpp"
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "SAudioSample.hpp"
#include "Glacier2/Sound/SWaveBankResourceData.hpp"
#include "Glacier2/Sound/SMusicCompositionData.hpp"
#include "Glacier2/ZBinaryDeserializer.hpp"

namespace HM5ResourceTool
{
    class __declspec(dllexport) ZAudio
    {
    public:
        static void ConvertFSBFToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size);
        static bool ConvertFSBFToOGG(const std::string& fsbfPath, const std::string& outputFilePath);
        static bool ConvertFSBFToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBFToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath);
        static bool ConvertFSBFToWAV(const std::string& fsbfPath, const std::string& outputFilePath);
        static bool ConvertFSBFToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBFToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath);

        static void ConvertFSBMToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size);
        static bool ConvertFSBMToOGG(const std::string& fsbmPath, const std::string& outputFilePath);
        static bool ConvertFSBMToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBMToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath);
        static bool ConvertFSBMToWAV(const std::string& fsbmPath, const std::string& outputFilePath);
        static bool ConvertFSBMToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBMToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath);

        static void ConvertFSBSToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size);
        static bool ConvertFSBSToOGG(const std::string& fsbsPath, const std::string& outputFilePath);
        static bool ConvertFSBSToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBSToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath);
        static bool ConvertFSBSToWAV(const std::string& fsbsPath, const std::string& outputFilePath);
        static bool ConvertFSBSToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples);
        static bool ConvertFSBSToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath);

        static void ConvertFSB5ToOGG(void* fsb5Data, unsigned int& fsb5Size, const std::string& outputFilePath);
        static void ConvertFSB5ToOGG(void* fsb5Data, unsigned int& fsb5Size, std::vector<SAudioSample*>& audioSamples);
        static void ConvertFSB5ToWAV(void* fsb5Data, unsigned int& fsb5Size, const std::string& outputFilePath);
        static void ConvertFSB5ToWAV(void* fsb5Data, unsigned int& fsb5Size, std::vector<SAudioSample*>& audioSamples);

        static bool ConvertFSB5ToFSBF(const std::string& fsb5Path);
        static bool ConvertFSB5ToFSBM(const std::string& fsb5Path);
        static bool ConvertFSB5ToFSBS(const std::string& fsb5Path);

        static bool ConvertWAVBToJSON(const std::string& wavbPath, std::string& jsonOutput);
        static bool ConvertWAVBToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
        static bool ConvertMUCBToJSON(const std::string& mucbPath, std::string& jsonOutput);
        static bool ConvertMUCBToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
        static bool ConvertJSONToWAVB(const std::string& jsonPath);
        static bool IsOGGFormat(const void* data, const unsigned int dataSize, unsigned int startOffset);
        static bool IsOGGFormat(IO::ZBinaryReader& binaryReader, unsigned int startOffset);
        static unsigned int FindFSB5DataOffsetInFSBS(IO::ZBinaryReader& binaryReader);
    };
}
