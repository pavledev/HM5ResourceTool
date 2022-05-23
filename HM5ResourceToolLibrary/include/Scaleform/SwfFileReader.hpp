#pragma once

#include <string>
#include <vector>
#include <limits>
#include "ISwfFileReader.hpp"
#include "IO/ZBinaryReader.hpp"
#include "SRect.hpp"

namespace SwfLib
{
    class SwfFileReader : public ISwfFileReader
    {
    public:
        SwfFileReader(const std::string& filePath);
        SwfFileReader(const void* buffer, const uintptr_t size);
        bool IsEOF() const override;
        long long Position() const override;
        long long Length() const override;

        double ReadFixedPoint8() override;
        double ReadFixed() override;
        double ReadFixedPoint16(unsigned int bits) override;
        unsigned short ReadUInt16() override;
        short ReadSInt16() override;
        unsigned int ReadUInt32() override;
        unsigned long long ReadUInt64() override;
        int ReadInt32() override;
        char* ReadChars(int size) override;
        unsigned char ReadByte() override;
        //unsigned char* ReadBytes(int count) override;
        void* ReadBytes(int count) override;
        void ReadBytes(void* buffer, const int size) override;
        void* ReadRest() override;
        long long BytesLeft() const override;

        /// <summary>
        /// Reads variable-length encoded unsigned 32-bit integer
        /// </summary>
        /// <returns></returns>
        unsigned int ReadEncodedU32() override;

        std::string ReadString() override;
        std::string ReadString(const int size) override;

        float ReadSingle() override;
        double ReadDouble() override;
        float ReadShortFloat() override;

        static SRect ReadRect(ISwfFileReader* reader);
        static void ReadRect(ISwfFileReader* reader, SRect& rect);

        struct BitContext
        {
            unsigned char cachedByte = 0;
            unsigned char bitIndex = 0;
        };

#pragma region Bit Reading

        bool ReadBit() override;
        int ReadSignedBits(unsigned int count) override;
        unsigned int ReadUnsignedBits(unsigned int count) override;
        void AlignToByte() override;

#pragma endregion Bit Reading

        void* GetCurrentPointer() override;
        void Seek(const uintptr_t offset, const HM5ResourceTool::IO::IInputStream::ESeekOrigin seekOrigin) override;

    private:
        HM5ResourceTool::IO::ZBinaryReader binaryReader;
        BitContext bitContext;
    };
}
