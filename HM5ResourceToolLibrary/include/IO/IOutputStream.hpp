#pragma once

#include <string>

namespace HM5ResourceTool
{
	namespace IO
	{
		class IOutputStream
		{
		public:
			enum class ESeekOrigin
			{
				Begin,
				Current,
				End
			};

			enum class Endian
			{
				big = 0,
				little = 1
			};

			virtual ~IOutputStream() = 0;
			virtual size_t GetCapacity() const = 0;
			virtual size_t GetPosition() = 0;
			virtual void* GetBuffer() const = 0;
			virtual void* GetCurrentPointer() const = 0;
			virtual void Close() = 0;
			virtual void Skip(const size_t count) = 0;
			virtual void Seek(const size_t offset, const ESeekOrigin seekOrigin) = 0;
			virtual void WriteChar(char value) = 0;
			virtual void WriteByte(unsigned char value) = 0;
			virtual void WriteInt16(short value) = 0;
			virtual void WriteInt32(int value) = 0;
			virtual void WriteInt64(long long value) = 0;
			virtual void WriteUInt16(unsigned short value) = 0;
			virtual void WriteUInt32(unsigned int value) = 0;
			virtual void WriteUInt64(unsigned long long value) = 0;
			virtual void WriteFloat(float value) = 0;
			virtual void WriteDouble(double value) = 0;
			virtual void WriteBool(bool value) = 0;
			virtual void WriteString(std::string string) = 0;
			virtual void WriteBytes(void* buffer, const size_t size) = 0;
			virtual void AlignTo(size_t alignment) = 0;
			virtual void SwapEndian(void* buffer, size_t size) = 0;
			virtual void SetEndian(const Endian endian) = 0;
		};
	}
}
