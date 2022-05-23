#include "pch.h"
#include "Scaleform/SwfFileReader.hpp"

namespace SwfLib
{
    SwfFileReader::SwfFileReader(const std::string& filePath)
    {
        binaryReader = HM5ResourceTool::IO::ZBinaryReader(filePath);
    }

    SwfFileReader::SwfFileReader(const void* buffer, const uintptr_t size)
    {
        binaryReader = HM5ResourceTool::IO::ZBinaryReader(buffer, size);
    }

    bool SwfFileReader::IsEOF() const
    {
        return binaryReader.GetInputStream()->GetPosition() == binaryReader.GetInputStream()->GetSize();
    }

    long long SwfFileReader::Position() const
    {
        return binaryReader.GetInputStream()->GetPosition();
    }

    long long SwfFileReader::Length() const
    {
        return binaryReader.GetInputStream()->GetSize();
    }

    double SwfFileReader::ReadFixedPoint8()
    {
        return ReadUInt16() / 256.0;
    }

    double SwfFileReader::ReadFixed()
    {
        return ReadInt32() / 65536.0;
    }

    double SwfFileReader::ReadFixedPoint16(unsigned int bits)
    {
        int value = ReadSignedBits(bits);

        return value / 65536.0;
    }

    unsigned short SwfFileReader::ReadUInt16()
    {
        return binaryReader.ReadUInt16();
    }

    short SwfFileReader::ReadSInt16()
    {
        return binaryReader.ReadInt16();
    }

    unsigned int SwfFileReader::ReadUInt32()
    {
        return binaryReader.ReadUInt32();
    }

    unsigned long long SwfFileReader::ReadUInt64()
    {
        return binaryReader.ReadUInt64();
    }

    int SwfFileReader::ReadInt32()
    {
        return binaryReader.ReadInt32();
    }

    char* SwfFileReader::ReadChars(int size)
    {
        return binaryReader.ReadChars(size);
    }

    unsigned char SwfFileReader::ReadByte()
    {
        AlignToByte();

        return binaryReader.ReadByte();
    }

    /*unsigned char* SwfFileReader::ReadBytes(int count)
    {
        return static_cast<unsigned char*>(binaryReader.ReadBytes(count));
    }*/

    void* SwfFileReader::ReadBytes(int count)
    {
        return binaryReader.ReadBytes(count);
    }

    void SwfFileReader::ReadBytes(void* buffer, const int size)
    {
        binaryReader.ReadBytes(buffer, size);
    }

    void* SwfFileReader::ReadRest()
    {
        return ReadBytes(static_cast<int>(BytesLeft()));
    }

    long long SwfFileReader::BytesLeft() const
    {
        return binaryReader.GetInputStream()->GetSize() - binaryReader.GetInputStream()->GetPosition();
    }

    /// <summary>
    /// Reads variable-length encoded unsigned 32-bit integer
    /// </summary>
    /// <returns></returns>
    unsigned int SwfFileReader::ReadEncodedU32()
    {
        AlignToByte();

        unsigned int val = 0;
        auto bt = binaryReader.ReadByte();

        val |= bt & 0x7fu;

        if ((bt & 0x80) == 0)
        {
            return val;
        }

        bt = binaryReader.ReadByte();
        val |= (bt & 0x7fu) << 7;

        if ((bt & 0x80) == 0)
        {
            return val;
        }

        bt = binaryReader.ReadByte();
        val |= (bt & 0x7fu) << 14;

        if ((bt & 0x80) == 0)
        {
            return val;
        }

        bt = binaryReader.ReadByte();
        val |= (bt & 0x7fu) << 21;

        if ((bt & 0x80) == 0)
        {
            return val;
        }

        bt = binaryReader.ReadByte();
        val |= (bt & 0x7fu) << 28;

        return val;
    }

    /// <summary>
    /// Reads Null-terminated string
    /// </summary>
    /// <returns></returns>
    std::string SwfFileReader::ReadString()
    {
        return binaryReader.ReadString();
    }

    std::string SwfFileReader::ReadString(const int size)
    {
        return binaryReader.ReadString(size);
    }

    float SwfFileReader::ReadSingle()
    {
        return binaryReader.ReadFloat();
    }

    double SwfFileReader::ReadDouble()
    {
        return binaryReader.ReadDouble();
    }

    float SwfFileReader::ReadShortFloat()
    {
        auto data = binaryReader.ReadUInt16();

        if (data == 0)
        {
            return 0;
        }

        auto sign = data & 0x8000;
        auto exp = (data & 0x7c00) >> 10;
        auto m = (data & 0x3ff) | 0x400;

        if (sign != 0)
        {
            m = -m;
        }

        auto k = 1 << exp;

        const float bias = 1 << 25;

        return (static_cast<float>(m)) * k / bias;
    }

    SRect SwfFileReader::ReadRect(ISwfFileReader* reader)
    {
        SRect rect;

        ReadRect(reader, rect);

        return rect;
    }

    void SwfFileReader::ReadRect(ISwfFileReader* reader, SRect& rect)
    {
        auto bits = reader->ReadUnsignedBits(5);

        rect.xmin = reader->ReadSignedBits(bits);
        rect.xmax = reader->ReadSignedBits(bits);
        rect.ymin = reader->ReadSignedBits(bits);
        rect.ymax = reader->ReadSignedBits(bits);

        reader->AlignToByte();
    }

    bool SwfFileReader::ReadBit()
    {
        auto bitIndex = bitContext.bitIndex & 0x07;

        if (bitIndex == 0)
        {
            bitContext.cachedByte = ReadByte();
        }

        bitContext.bitIndex++;

        return ((bitContext.cachedByte << bitIndex) & 0x80) != 0;
    }

    int SwfFileReader::ReadSignedBits(unsigned int count)
    {
        if (count == 0) return 0;

        bool sign = ReadBit();
        auto res = sign ? UINT_MAX : 0;

        count--;

        for (unsigned int i = 0; i < count; i++)
        {
            auto bit = ReadBit();

            res = (res << 1 | (bit ? 1u : 0u));
        }

        return (int)res;
    }

    unsigned int SwfFileReader::ReadUnsignedBits(unsigned int count)
    {
        if (count == 0) return 0;

        unsigned int res = 0;

        for (unsigned int i = 0; i < count; i++)
        {
            auto bit = ReadBit();

            res = (res << 1 | (bit ? 1u : 0u));
        }

        return res;
    }

    void SwfFileReader::AlignToByte()
    {
        bitContext.bitIndex = 0;
        bitContext.cachedByte = 0;
    }

    void* SwfFileReader::GetCurrentPointer()
    {
        return binaryReader.GetInputStream()->GetCurrentPointer();
    }

    void SwfFileReader::Seek(const uintptr_t offset, const HM5ResourceTool::IO::IInputStream::ESeekOrigin seekOrigin)
    {
        return binaryReader.GetInputStream()->Seek(offset, seekOrigin);
    }
}
