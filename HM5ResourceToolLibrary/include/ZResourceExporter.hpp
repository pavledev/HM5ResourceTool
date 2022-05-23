#pragma once

#include <string>

namespace HM5ResourceTool
{
    class ZResource;

    class ZResourceExporter
    {
    public:
        static bool WriteJSONToFile(const std::string& jsonOutput, const std::string& filePath);
        static bool WriteToTextFile(const std::string& output, const std::string& filePath);

        __declspec(dllexport) static bool ExportTEMP(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTBLU(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportCPPT(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportCBLU(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportAIBB(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTEXT(const std::string& inputFilePath, const std::string& outputFilePath, unsigned int option);
        __declspec(dllexport) static bool ExportSWFF(const std::string& inputFilePath, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportGFXF(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBF(const std::string& inputFilePath, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBM(const std::string& inputFilePath, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBS(const std::string& inputFilePath, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportWAVB(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMUCB(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTELI(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportPRIM(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATI(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATT(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATB(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTEMP(ZResource* resource, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTBLU(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportCPPT(ZResource* resource, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportCBLU(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportAIBB(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTEXT(const void* data, const unsigned int dataSize, const std::string& outputFilePath, unsigned int option);
        __declspec(dllexport) static bool ExportSWFF(const void* data, const unsigned int dataSize, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportGFXF(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBF(const void* data, const unsigned int dataSize, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBM(const void* data, const unsigned int dataSize, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportFSBS(const void* data, const unsigned int dataSize, std::string& outputFilePath);
        __declspec(dllexport) static bool ExportWAVB(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMUCB(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportTELI(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportPRIM(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATI(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATT(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportMATB(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportPCBinkVid(const std::string& inputFilePath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ExportPackageList(const std::string& inputFilePath, const std::string& outputFilePath);
    };
}
