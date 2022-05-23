#pragma once

#include <vector>
#include <string>
#include "IO/ZBinaryReader.hpp"

namespace SwfLib
{
	class ISwfFileReader
	{
	public:
		virtual bool IsEOF() const = 0;
		virtual long long Position() const = 0;
		virtual long long Length() const = 0;
		virtual long long BytesLeft() const = 0;

		virtual double ReadFixedPoint8() = 0;
		virtual double ReadFixed() = 0;
		virtual double ReadFixedPoint16(unsigned int bits) = 0;

		virtual unsigned short ReadUInt16() = 0;
		virtual short ReadSInt16() = 0;

		virtual unsigned int ReadUInt32() = 0;
		virtual int ReadInt32() = 0;

		virtual unsigned long long ReadUInt64() = 0;

		virtual unsigned int ReadEncodedU32() = 0;

		virtual char* ReadChars(int count) = 0;
		virtual unsigned char ReadByte() = 0;
		//virtual unsigned char* ReadBytes(int count) = 0;
		virtual void* ReadBytes(int count) = 0;
		virtual void ReadBytes(void* buffer, const int size) = 0;
		virtual void* ReadRest() = 0;

		virtual std::string ReadString() = 0;
		virtual std::string ReadString(const int size) = 0;

		virtual float ReadSingle() = 0;
		virtual double ReadDouble() = 0;
		virtual float ReadShortFloat() = 0;

		virtual bool ReadBit() = 0;
		virtual int ReadSignedBits(unsigned int count) = 0;
		virtual unsigned int ReadUnsignedBits(unsigned int count) = 0;
		virtual void AlignToByte() = 0;

		virtual void* GetCurrentPointer() = 0;
		virtual void Seek(const uintptr_t offset, const HM5ResourceTool::IO::IInputStream::ESeekOrigin seekOrigin) = 0;
	};
}
