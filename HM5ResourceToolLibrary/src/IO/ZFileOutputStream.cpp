#include "pch.h"
#include "IO/ZFileOutputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZFileOutputStream::ZFileOutputStream()
		{
			endian = Endian::little;
		}

		ZFileOutputStream::ZFileOutputStream(const std::string& filePath, Endian endian)
		{
			outputFileStream = std::ofstream(filePath, std::ios::binary);

			outputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			this->endian = endian;
		}

		ZFileOutputStream::~ZFileOutputStream()
		{
			if (outputFileStream.is_open())
			{
				outputFileStream.close();
			}
		}

		size_t ZFileOutputStream::GetCapacity() const
		{
			return 0;
		}

		size_t ZFileOutputStream::GetPosition()
		{
			return static_cast<size_t>(outputFileStream.tellp());
		}

		void* ZFileOutputStream::GetBuffer() const
		{
			return nullptr;
		}

		void* ZFileOutputStream::GetCurrentPointer() const
		{
			return nullptr;
		}

		void ZFileOutputStream::Close()
		{
			if (outputFileStream.is_open())
			{
				outputFileStream.close();
			}
		}

		void ZFileOutputStream::Skip(const size_t count)
		{
			outputFileStream.seekp(count, outputFileStream.cur);
		}

		void ZFileOutputStream::Seek(const size_t offset, const ESeekOrigin seekOrigin)
		{
			switch (seekOrigin)
			{
			case ESeekOrigin::Begin:
				outputFileStream.seekp(offset, outputFileStream.beg);
				break;
			case ESeekOrigin::Current:
				outputFileStream.seekp(offset, outputFileStream.cur);
				break;
			case ESeekOrigin::End:
				outputFileStream.seekp(offset, outputFileStream.end);
				break;
			default:
				break;
			}
		}

		void ZFileOutputStream::WriteChar(char value)
		{
			outputFileStream.write(&value, sizeof(value));
		}

		void ZFileOutputStream::WriteByte(unsigned char value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteInt16(short value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteInt32(int value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteInt64(long long value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteUInt16(unsigned short value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteUInt32(unsigned int value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteUInt64(unsigned long long value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteFloat(float value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteDouble(double value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteBool(bool value)
		{
			outputFileStream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void ZFileOutputStream::WriteString(std::string string)
		{
			outputFileStream.write(string.data(), string.length());
		}

		void ZFileOutputStream::WriteBytes(void* buffer, const size_t size)
		{
			outputFileStream.write(static_cast<const char*>(buffer), size);
		}

		void ZFileOutputStream::AlignTo(size_t alignment)
		{

		}

		void ZFileOutputStream::SwapEndian(void* buffer, size_t size)
		{
			unsigned char* buffer2 = static_cast<unsigned char*>(buffer);

			std::reverse(buffer2, buffer2 + size);
		}

		void ZFileOutputStream::SetEndian(const Endian endian)
		{
			this->endian = endian;
		}
	}
}
