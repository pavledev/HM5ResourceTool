#include "pch.h"
#include "IO/ZMemoryInputStream.hpp"

namespace HM5ResourceTool
{
    namespace IO
    {
        ZMemoryInputStream::ZMemoryInputStream()
        {
            this->buffer = nullptr;
            this->size = 0;
            this->position = 0;
            this->endian = Endian::little;
        }

        ZMemoryInputStream::ZMemoryInputStream(const void* buffer, const size_t size, Endian endian)
        {
            this->buffer = const_cast<void*>(buffer);
            this->size = size;
            this->position = 0;
            this->endian = endian;
        }

        ZMemoryInputStream::~ZMemoryInputStream()
        {

        }

        size_t ZMemoryInputStream::GetSize() const
        {
            return size;
        }

        size_t ZMemoryInputStream::GetPosition()
        {
            return position;
        }

        void* ZMemoryInputStream::GetBuffer() const
        {
            return buffer;
        }

        void* ZMemoryInputStream::GetCurrentPointer() const
        {
            return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + position);
        }

        void ZMemoryInputStream::Close()
        {

        }

        void ZMemoryInputStream::Skip(const size_t count)
        {
            Seek(count, ESeekOrigin::Current);
        }

        void ZMemoryInputStream::Seek(const size_t offset, const ESeekOrigin seekOrigin)
        {
            switch (seekOrigin)
            {
            case ESeekOrigin::Begin:
            {
                assert(position <= size);

                position = offset;

                break;
            }
            case ESeekOrigin::Current:
            {
                assert(position + offset <= size);

                position += offset;

                break;
            }
            case ESeekOrigin::End:
            {
                assert(position + offset >= 0);

                position = size - offset;

                break;
            }
            }
        }

        char ZMemoryInputStream::ReadChar()
        {
            assert(position + sizeof(char) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(char);

            char value = *reinterpret_cast<char*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(char));
            }

            return value;
        }

        unsigned char ZMemoryInputStream::ReadByte()
        {
            assert(position + sizeof(unsigned char) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(unsigned char);

            unsigned char value = *reinterpret_cast<unsigned char*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(unsigned char));
            }

            return value;
        }

        short ZMemoryInputStream::ReadInt16()
        {
            assert(position + sizeof(short) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(short);

            short value = *reinterpret_cast<short*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(short));
            }

            return value;
        }

        int ZMemoryInputStream::ReadInt32()
        {
            assert(position + sizeof(int) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(int);

            int value = *reinterpret_cast<int*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(int));
            }

            return value;
        }

        long long ZMemoryInputStream::ReadInt64()
        {
            assert(position + sizeof(long long) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(long long);

            long long value = *reinterpret_cast<long long*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(long long));
            }

            return value;
        }

        unsigned short ZMemoryInputStream::ReadUInt16()
        {
            assert(position + sizeof(unsigned short) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(unsigned short);

            unsigned short value = *reinterpret_cast<unsigned short*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(unsigned short));
            }

            return value;
        }

        unsigned int ZMemoryInputStream::ReadUInt32()
        {
            assert(position + sizeof(unsigned int) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(unsigned int);

            unsigned int value = *reinterpret_cast<unsigned int*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(unsigned int));
            }

            return value;
        }

        unsigned long long ZMemoryInputStream::ReadUInt64()
        {
            assert(position + sizeof(unsigned long long) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(unsigned long long);

            unsigned long long value = *reinterpret_cast<unsigned long long*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(unsigned long long));
            }

            return value;
        }

        float ZMemoryInputStream::ReadFloat()
        {
            assert(position + sizeof(float) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(float);

            float value = *reinterpret_cast<float*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(float));
            }

            return value;
        }

        double ZMemoryInputStream::ReadDouble()
        {
            assert(position + sizeof(double) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(double);

            double value = *reinterpret_cast<double*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(double));
            }

            return value;
        }

        bool ZMemoryInputStream::ReadBool()
        {
            assert(position + sizeof(bool) <= size);

            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(buffer) + position;

            position += sizeof(bool);

            bool value = *reinterpret_cast<bool*>(bufferPointer);

            if (endian == Endian::big)
            {
                SwapEndian(&value, sizeof(bool));
            }

            return value;
        }

        char* ZMemoryInputStream::ReadChars(const size_t size)
        {
            assert(position + size <= this->size);

            char* buffer = new char[size + 1];
            uintptr_t bufferPointer = reinterpret_cast<uintptr_t>(this->buffer) + position;

            memcpy(buffer, reinterpret_cast<char*>(bufferPointer), size);

            position += size;

            if (endian == Endian::big)
            {
                SwapEndian(buffer, size);
            }

            return buffer;
        }

        std::string ZMemoryInputStream::ReadString()
        {
            char c;
            std::string result;

            while ((c = ReadChar()) != '\0')
            {
                result += c;
            }

            /*if (endian == Endian::big)
            {
                SwapEndian(result.data(), result.length());
            }*/

            return result;
        }

        std::string ZMemoryInputStream::ReadString(const size_t size)
        {
            char* data = static_cast<char*>(ReadBytes(size + 1));

            /*if (endian == Endian::big)
            {
                SwapEndian(data, size);
            }*/

            std::string result = std::string(data, size);

            delete[] data;

            return result;
        }

        void* ZMemoryInputStream::ReadBytes(const size_t size)
        {
            assert(position + size <= this->size);

            void* buffer = operator new(size);

            memcpy(buffer, GetCurrentPointer(), size);

            position += size;

            if (endian == Endian::big)
            {
                SwapEndian(buffer, size);
            }

            return buffer;
        }

        void ZMemoryInputStream::ReadBytes(void* buffer, const size_t size)
        {
            assert(position + size <= this->size);

            memcpy(buffer, GetCurrentPointer(), size);

            /*if (endian == Endian::big)
            {
                SwapEndian(buffer, size);
            }*/

            position += size;
        }

        IOI::SVector4 ZMemoryInputStream::ReadVector4()
        {
            float x = ((2.0f * static_cast<float>(ReadByte())) / 255.0f) - 1.0f;
            float y = ((2.0f * static_cast<float>(ReadByte())) / 255.0f) - 1.0f;
            float z = ((2.0f * static_cast<float>(ReadByte())) / 255.0f) - 1.0f;
            float w = ((2.0f * static_cast<float>(ReadByte())) / 255.0f) - 1.0f;

            if (endian == Endian::big)
            {
                return IOI::SVector4(w, z, y, x);
            }

            return IOI::SVector4(x, y, z, w);
        }

        void ZMemoryInputStream::SwapEndian(void* buffer, size_t size)
        {
            unsigned char* buffer2 = static_cast<unsigned char*>(buffer);

            std::reverse(buffer2, buffer2 + size);
        }

        void ZMemoryInputStream::SetEndian(const Endian endian)
        {
            this->endian = endian;
        }
    }
}
