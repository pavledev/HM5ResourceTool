#include "pch.h"
#include <fstream>
#include <iostream>
#include "IO/ZBinaryReader.hpp"
#include "IO/ZFileInputStream.hpp"
#include "IO/ZMemoryInputStream.hpp"

namespace HM5ResourceTool
{
    namespace IO
    {
        ZBinaryReader::ZBinaryReader()
        {
            inputStream = nullptr;
        }

        ZBinaryReader::ZBinaryReader(const std::string& filePath, IInputStream::Endian endian)
        {
            inputStream = new ZFileInputStream(filePath, endian);
        }

        ZBinaryReader::ZBinaryReader(const void* buffer, const size_t size, IInputStream::Endian endian)
        {
            inputStream = new ZMemoryInputStream(buffer, size);
        }

        ZBinaryReader::~ZBinaryReader()
        {
            if (inputStream)
            {
                delete inputStream;
            }
        }

        IInputStream* ZBinaryReader::GetInputStream()
        {
            return inputStream;
        }

        IInputStream* ZBinaryReader::GetInputStream() const
        {
            return inputStream;
        }

        char ZBinaryReader::ReadChar()
        {
            return inputStream->ReadChar();
        }

        unsigned char ZBinaryReader::ReadByte()
        {
            return inputStream->ReadByte();
        }

        short ZBinaryReader::ReadInt16()
        {
            return inputStream->ReadInt16();
        }

        int ZBinaryReader::ReadInt32()
        {
            return inputStream->ReadInt32();
        }

        long long ZBinaryReader::ReadInt64()
        {
            return inputStream->ReadInt64();
        }

        unsigned short ZBinaryReader::ReadUInt16()
        {
            return inputStream->ReadUInt16();
        }

        unsigned int ZBinaryReader::ReadUInt32()
        {
            return inputStream->ReadUInt32();
        }

        unsigned long long ZBinaryReader::ReadUInt64()
        {
            return inputStream->ReadUInt64();
        }

        float ZBinaryReader::ReadFloat()
        {
            return inputStream->ReadFloat();
        }

        double ZBinaryReader::ReadDouble()
        {
            return inputStream->ReadDouble();
        }

        bool ZBinaryReader::ReadBool()
        {
            return inputStream->ReadBool();
        }

        char* ZBinaryReader::ReadChars(const size_t size)
        {
            return inputStream->ReadChars(size);
        }

        std::string ZBinaryReader::ReadString()
        {
            return inputStream->ReadString();
        }

        std::string ZBinaryReader::ReadString(const size_t size)
        {
            return inputStream->ReadString(size);
        }

        void* ZBinaryReader::ReadBytes(const size_t size)
        {
            return inputStream->ReadBytes(size);
        }

        void ZBinaryReader::ReadBytes(void* buffer, const size_t size)
        {
            return inputStream->ReadBytes(buffer, size);
        }

        IOI::SVector4 ZBinaryReader::ReadVector4()
        {
            return inputStream->ReadVector4();
        }

        void ZBinaryReader::SetEndian(const IInputStream::Endian endian)
        {
            inputStream->SetEndian(endian);
        }
    }
}
