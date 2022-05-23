#include "pch.h"
#include "ZTexture.hpp"

namespace HM5ResourceToolCLI
{
    ZTexture::ZTexture() : ManagedObject(new HM5ResourceTool::ZTexture())
    {

    }

    bool ZTexture::ConvertTextureMapToDDS(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% ddsData, [Out] unsigned int% ddsDataSize)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        void* ddsData2 = nullptr;
        unsigned int ddsSize2 = 0;

        bool result = HM5ResourceTool::ZTexture::ConvertTextureMapToDDS(dataPointer, dataSize, &ddsData2, ddsSize2);
        IntPtr ddsDataPointer = IntPtr(ddsData2);

        ddsDataSize = ddsSize2;
        ddsData = gcnew array<Byte>(ddsDataSize);

        Marshal::Copy(ddsDataPointer, ddsData, 0, ddsDataSize);

        return result;
    }

    bool ZTexture::ConvertTextureMapToTGA(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% tgaData, [Out] unsigned int% tgaDataSize)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        void* tgaData2 = nullptr;
        unsigned int tgaSize2 = 0;

        bool result = HM5ResourceTool::ZTexture::ConvertTextureMapToTGA(dataPointer, dataSize, &tgaData2, tgaSize2);
        IntPtr tgaDataPointer = IntPtr(tgaData2);

        tgaDataSize = tgaSize2;
        tgaData = gcnew array<Byte>(tgaDataSize);

        Marshal::Copy(tgaDataPointer, tgaData, 0, tgaDataSize);

        return result;
    }

    bool ZTexture::ConvertTextureMapToBMP(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% bmpData, [Out] unsigned int% bmpDataSize)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        void* bmpData2 = nullptr;
        unsigned int bmpSize2 = 0;

        bool result = HM5ResourceTool::ZTexture::ConvertTextureMapToBMP(dataPointer, dataSize, &bmpData2, bmpSize2);
        IntPtr bmpDataPointer = IntPtr(bmpData2);

        bmpDataSize = bmpSize2;
        bmpData = gcnew array<Byte>(bmpDataSize);

        Marshal::Copy(bmpDataPointer, bmpData, 0, bmpDataSize);

        return result;
    }

    bool ZTexture::ConvertTextureMapToPNG(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% pngData, [Out] unsigned int% pngDataSize)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];
        void* pngData2 = nullptr;
        unsigned int pngSize2 = 0;

        bool result = HM5ResourceTool::ZTexture::ConvertTextureMapToPNG(dataPointer, dataSize, &pngData2, pngSize2);
        IntPtr pngDataPointer = IntPtr(pngData2);

        pngDataSize = pngSize2;
        pngData = gcnew array<Byte>(pngDataSize);

        Marshal::Copy(pngDataPointer, pngData, 0, pngDataSize);

        return result;
    }

    /*bool ZTexture::ConvertTGAToTextureMap(String^ tgaPath)
    {
        std::string tgaPath2 = marshal_as<std::string>(tgaPath);

        return HM5ResourceTool::ZTexture::ConvertDDSToTextureMap(tgaPath2);
    }

    bool ZTexture::ConvertBMPToTextureMap(String^ bmpPath)
    {
        std::string bmpPath2 = marshal_as<std::string>(bmpPath);

        return HM5ResourceTool::ZTexture::ConvertDDSToTextureMap(bmpPath2);
    }

    bool ZTexture::ConvertPNGToTextureMap(String^ pngPath)
    {
        std::string pngPath2 = marshal_as<std::string>(pngPath);

        return HM5ResourceTool::ZTexture::ConvertDDSToTextureMap(pngPath2);
    }*/

    String^ ZTexture::GetErrorMessage(HRESULT hresult)
    {
        return marshal_as<String^>(HM5ResourceTool::ZTexture::GetErrorMessage(hresult));
    }
}
