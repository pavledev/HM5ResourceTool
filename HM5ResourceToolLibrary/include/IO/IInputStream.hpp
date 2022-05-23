#pragma once

#include <string>
#include "../Glacier2/Math/SVector4.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		class IInputStream
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

			virtual ~IInputStream() = 0;
			virtual size_t GetSize() const = 0;
			virtual size_t GetPosition() = 0;
			virtual void* GetBuffer() const = 0;
			virtual void* GetCurrentPointer() const = 0;
			virtual void Close() = 0;
			virtual void Skip(const size_t count) = 0;
			virtual void Seek(const size_t offset, const ESeekOrigin seekOrigin) = 0;
			virtual char ReadChar() = 0;
			virtual unsigned char ReadByte() = 0;
			virtual short ReadInt16() = 0;
			virtual int ReadInt32() = 0;
			virtual long long ReadInt64() = 0;
			virtual unsigned short ReadUInt16() = 0;
			virtual unsigned int ReadUInt32() = 0;
			virtual unsigned long long ReadUInt64() = 0;
			virtual float ReadFloat() = 0;
			virtual double ReadDouble() = 0;
			virtual bool ReadBool() = 0;
			virtual char* ReadChars(const size_t size) = 0;
			virtual std::string ReadString() = 0;
			virtual std::string ReadString(const size_t size) = 0;
			virtual void* ReadBytes(const size_t size) = 0;
			virtual void ReadBytes(void* buffer, const size_t size) = 0;
			virtual IOI::SVector4 ReadVector4() = 0;
			virtual void SwapEndian(void* buffer, size_t size) = 0;
			virtual void SetEndian(const Endian endian) = 0;
		};
	}
}
