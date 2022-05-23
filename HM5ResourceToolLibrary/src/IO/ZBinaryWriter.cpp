#include "pch.h"
#include "IO/ZBinaryWriter.hpp"
#include "IO/ZFileOutputStream.hpp"
#include "IO/ZMemoryOuputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZBinaryWriter::ZBinaryWriter()
		{
			outputStream = nullptr;
		}

		ZBinaryWriter::ZBinaryWriter(const std::string& filePath, IOutputStream::Endian endian)
		{
			outputStream = new ZFileOutputStream(filePath, endian);
		}

		ZBinaryWriter::ZBinaryWriter(const size_t capacity, IOutputStream::Endian endian)
		{
			outputStream = new ZMemoryOuputStream(capacity, endian);
		}

		ZBinaryWriter::ZBinaryWriter(const void* buffer, const size_t size, IOutputStream::Endian endian)
		{
			outputStream = new ZMemoryOuputStream(buffer, size, endian);
		}

		ZBinaryWriter::~ZBinaryWriter()
		{
			if (outputStream)
			{
				delete outputStream;
			}
		}

		IOutputStream* ZBinaryWriter::GetOutputStream()
		{
			return outputStream;
		}

		void ZBinaryWriter::WriteChar(const char value)
		{
			outputStream->WriteChar(value);
		}

		void ZBinaryWriter::WriteByte(const unsigned char value)
		{
			outputStream->WriteByte(value);
		}

		void ZBinaryWriter::WriteInt16(const short value)
		{
			outputStream->WriteInt16(value);
		}

		void ZBinaryWriter::WriteInt32(const int value)
		{
			outputStream->WriteInt32(value);
		}

		void ZBinaryWriter::WriteInt64(const long long value)
		{
			outputStream->WriteInt64(value);
		}

		void ZBinaryWriter::WriteUInt16(const unsigned short value)
		{
			outputStream->WriteUInt16(value);
		}

		void ZBinaryWriter::WriteUInt32(const unsigned int value)
		{
			outputStream->WriteUInt32(value);
		}

		void ZBinaryWriter::WriteUInt64(const unsigned long long value)
		{
			outputStream->WriteUInt64(value);
		}

		void ZBinaryWriter::WriteFloat(const float value)
		{
			outputStream->WriteFloat(value);
		}

		void ZBinaryWriter::WriteDouble(const double value)
		{
			outputStream->WriteDouble(value);
		}

		void ZBinaryWriter::WriteBool(const bool value)
		{
			outputStream->WriteBool(value);
		}

		void ZBinaryWriter::WriteString(const std::string string)
		{
			outputStream->WriteString(string);
		}

		void ZBinaryWriter::WriteBytes(void* buffer, const size_t size)
		{
			outputStream->WriteBytes(buffer, size);
		}

		void ZBinaryWriter::AlignTo(size_t alignment)
		{
			outputStream->AlignTo(alignment);
		}

		void ZBinaryWriter::SetEndian(const IOutputStream::Endian endian)
		{
			outputStream->SetEndian(endian);
		}
	}
}
