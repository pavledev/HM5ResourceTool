#pragma once

#include <string>
#include "IInputStream.hpp"

namespace HM5ResourceTool
{
    namespace IO
    {
        class ZBinaryReader
        {
        public:
            ZBinaryReader();
            ZBinaryReader(const std::string& filePath, IInputStream::Endian endian = IInputStream::Endian::little);
            ZBinaryReader(const void* buffer, const size_t size, IInputStream::Endian endian = IInputStream::Endian::little);
            ~ZBinaryReader();
            IInputStream* GetInputStream();
            IInputStream* GetInputStream() const;
            char ReadChar();
            unsigned char ReadByte();
            short ReadInt16();
            int ReadInt32();
            long long ReadInt64();
            unsigned short ReadUInt16();
            unsigned int ReadUInt32();
            unsigned long long ReadUInt64();
            float ReadFloat();
            double ReadDouble();
            bool ReadBool();
            char* ReadChars(const size_t size);
            std::string ReadString();
            std::string ReadString(const size_t size);
            void* ReadBytes(const size_t size);
            void ReadBytes(void* buffer, const size_t size);
            IOI::SVector4 ReadVector4();
            void SetEndian(const IInputStream::Endian endian);

        private:
            IInputStream* inputStream;
        };
    }
}
