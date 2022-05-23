#pragma once

#include <string>
#include "IOutputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZBinaryWriter
		{
		public:
			ZBinaryWriter();
			ZBinaryWriter(const std::string& filePath, IOutputStream::Endian endian = IOutputStream::Endian::little);
			ZBinaryWriter(const size_t capacity, IOutputStream::Endian endian = IOutputStream::Endian::little);
			ZBinaryWriter(const void* buffer, const size_t size, IOutputStream::Endian endian = IOutputStream::Endian::little);
			~ZBinaryWriter();
			IOutputStream* GetOutputStream();
			void WriteChar(const char value);
			void WriteByte(const unsigned char value);
			void WriteInt16(const short value);
			void WriteInt32(const int value);
			void WriteInt64(const long long value);
			void WriteUInt16(const unsigned short value);
			void WriteUInt32(const unsigned int value);
			void WriteUInt64(const unsigned long long value);
			void WriteFloat(const float value);
			void WriteDouble(const double value);
			void WriteBool(const bool value);
			void WriteString(const std::string string);
			void WriteBytes(void* buffer, const size_t size);
			void AlignTo(size_t alignment);
			void SetEndian(const IOutputStream::Endian endian);

		private:
			IOutputStream* outputStream;
		};
	}
}
