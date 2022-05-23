#pragma once

#include <string>
#include "ZResource.hpp"

namespace HM5ResourceTool
{
    class ZResourceImporter
    {
    public:
        __declspec(dllexport) ZResourceImporter(const std::string& metaDataFilePath, const std::string& gamePath);
        __declspec(dllexport) ~ZResourceImporter();
        __declspec(dllexport) bool ImportResource(const std::string& inputFilePath, bool backupResourceLibrary);
        bool ImportTEMP(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportTBLU(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportCPPT(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportCBLU(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportAIBB(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportTEXT(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        /*bool ImportSWFF(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportGFXF(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportFSBF(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportFSBM(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportFSBS(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportWAVB(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportMUCB(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportTELI(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportPRIM(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportMATI(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportMATT(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportMATB(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportPCBinkVid(const std::string& inputFilePath, void** data, unsigned int& dataSize);
        bool ImportPackageList(const std::string& inputFilePath, void** data, unsigned int& dataSize);*/

    private:
        ZResource* resource;
        std::string gamePath;
    };
}
