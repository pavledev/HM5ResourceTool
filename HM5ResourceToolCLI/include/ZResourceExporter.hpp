#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZResourceExporter.hpp"
#include "ZResource.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZResourceExporter : public ManagedObject<HM5ResourceTool::ZResourceExporter>
    {
    public:
        static bool ExportTEMP(String^ inputFilePath, String^ outputFilePath);
        static bool ExportTBLU(String^ inputFilePath, String^ outputFilePath);
        static bool ExportCPPT(String^ inputFilePath, String^ outputFilePath);
        static bool ExportCBLU(String^ inputFilePath, String^ outputFilePath);
        static bool ExportAIBB(String^ inputFilePath, String^ outputFilePath);
        static bool ExportTEXT(String^ inputFilePath, String^ outputFilePath, unsigned int option);
        static bool ExportSWFF(String^ inputFilePath, String^ outputFilePath);
        static bool ExportGFXF(String^ inputFilePath, String^ outputFilePath);
        static bool ExportFSBF(String^ inputFilePath, String^ outputFilePath);
        static bool ExportFSBM(String^ inputFilePath, String^ outputFilePath);
        static bool ExportFSBS(String^ inputFilePath, String^ outputFilePath);
        static bool ExportWAVB(String^ inputFilePath, String^ outputFilePath);
        static bool ExportMUCB(String^ inputFilePath, String^ outputFilePath);
        static bool ExportTELI(String^ inputFilePath, String^ outputFilePath);
        static bool ExportPRIM(String^ inputFilePath, String^ outputFilePath);
        static bool ExportMATI(String^ inputFilePath, String^ outputFilePath);
        static bool ExportMATT(String^ inputFilePath, String^ outputFilePath);
        static bool ExportMATB(String^ inputFilePath, String^ outputFilePath);
        static bool ExportTEMP(ZResource^ resource, String^ outputFilePath);
        static bool ExportTBLU(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportCPPT(ZResource^ resource, String^ outputFilePath);
        static bool ExportCBLU(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportAIBB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportTEXT(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath, unsigned int option);
        static bool ExportSWFF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportGFXF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportFSBF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportFSBM(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportFSBS(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportWAVB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportMUCB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportTELI(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportPRIM(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportMATI(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportMATT(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportMATB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath);
        static bool ExportPCBinkVid(String^ inputFilePath, String^ outputFilePath);
        static bool ExportPackageList(String^ inputFilePath, String^ outputFilePath);
    };
}
