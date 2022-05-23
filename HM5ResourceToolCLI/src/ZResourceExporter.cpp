#include "pch.h"
#include "ZResourceExporter.hpp"

namespace HM5ResourceToolCLI
{
    bool ZResourceExporter::ExportTEMP(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTEMP(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportTBLU(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTBLU(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportCPPT(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportCPPT(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportCBLU(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportCBLU(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportAIBB(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportAIBB(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportTEXT(String^ inputFilePath, String^ outputFilePath, unsigned int option)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTEXT(inputFilePath2, outputFilePath2, option);
    }

    bool ZResourceExporter::ExportSWFF(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportSWFF(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportGFXF(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportGFXF(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBF(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBF(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBM(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBM(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBS(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBS(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportWAVB(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportWAVB(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportMUCB(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMUCB(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportTELI(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTELI(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportPRIM(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportPRIM(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATI(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATI(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATT(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATT(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATB(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATB(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportTEMP(ZResource^ resource, String^ outputFilePath)
    {
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTEMP(resource->GetInstance(), outputFilePath2);
    }

    bool ZResourceExporter::ExportTBLU(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTBLU(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportCPPT(ZResource^ resource, String^ outputFilePath)
    {
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportCPPT(resource->GetInstance(), outputFilePath2);
    }

    bool ZResourceExporter::ExportCBLU(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportCBLU(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportAIBB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportAIBB(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportTEXT(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath, unsigned int option)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTEXT(dataPointer, dataSize, outputFilePath2, option);
    }

    bool ZResourceExporter::ExportSWFF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportSWFF(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportGFXF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportGFXF(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBF(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBF(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBM(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBM(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportFSBS(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportFSBS(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportWAVB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportWAVB(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportMUCB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMUCB(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportTELI(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportTELI(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportPRIM(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportPRIM(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATI(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATI(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATT(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATT(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportMATB(const array<System::Byte>^ data, const unsigned int dataSize, String^ outputFilePath)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportMATB(dataPointer, dataSize, outputFilePath2);
    }

    bool ZResourceExporter::ExportPCBinkVid(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportPCBinkVid(inputFilePath2, outputFilePath2);
    }

    bool ZResourceExporter::ExportPackageList(String^ inputFilePath, String^ outputFilePath)
    {
        std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);
        std::string outputFilePath2 = marshal_as<std::string>(outputFilePath);

        return HM5ResourceTool::ZResourceExporter::ExportPackageList(inputFilePath2, outputFilePath2);
    }
}
