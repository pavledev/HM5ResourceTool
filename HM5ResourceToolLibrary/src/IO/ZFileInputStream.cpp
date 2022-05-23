#include "pch.h"
#include "IO/ZFileInputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZFileInputStream::ZFileInputStream()
		{
			size = 0;
			endian = Endian::little;
		}

		ZFileInputStream::ZFileInputStream(const std::string& filePath, Endian endian)
		{
			inputFileStream = std::ifstream(filePath, std::ios::binary);

			inputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			inputFileStream.seekg(0, std::ios_base::end);

			size = static_cast<size_t>(inputFileStream.tellg());

			inputFileStream.seekg(0, std::ios_base::beg);

			this->endian = endian;
		}

		ZFileInputStream::~ZFileInputStream()
		{
			if (inputFileStream.is_open())
			{
				inputFileStream.close();
			}
		}

		size_t ZFileInputStream::GetSize() const
		{
			return size;
		}

		size_t ZFileInputStream::GetPosition()
		{
			return static_cast<size_t>(inputFileStream.tellg());
		}

		void* ZFileInputStream::GetBuffer() const
		{
			return inputFileStream.rdbuf();
		}

		void* ZFileInputStream::GetCurrentPointer() const
		{
			return nullptr;
		}

		void ZFileInputStream::Close()
		{
			if (inputFileStream.is_open())
			{
				inputFileStream.close();
			}
		}

		void ZFileInputStream::Skip(const size_t count)
		{
			inputFileStream.seekg(count, inputFileStream.cur);
		}

		void ZFileInputStream::Seek(const size_t offset, const ESeekOrigin seekOrigin)
		{
			switch (seekOrigin)
			{
			case ESeekOrigin::Begin:
				inputFileStream.seekg(offset, inputFileStream.beg);
				break;
			case ESeekOrigin::Current:
				inputFileStream.seekg(offset, inputFileStream.cur);
				break;
			case ESeekOrigin::End:
				inputFileStream.seekg(offset, inputFileStream.end);
				break;
			default:
				break;
			}
		}

		char ZFileInputStream::ReadChar()
		{
			char value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(char));
			}

			return value;
		}

		unsigned char ZFileInputStream::ReadByte()
		{
			unsigned char value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		short ZFileInputStream::ReadInt16()
		{
			short value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		int ZFileInputStream::ReadInt32()
		{
			int value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		long long ZFileInputStream::ReadInt64()
		{
			long long value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		unsigned short ZFileInputStream::ReadUInt16()
		{
			unsigned short value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		unsigned int ZFileInputStream::ReadUInt32()
		{
			unsigned int value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		unsigned long long ZFileInputStream::ReadUInt64()
		{
			unsigned long long value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		float ZFileInputStream::ReadFloat()
		{
			float value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		double ZFileInputStream::ReadDouble()
		{
			double value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		bool ZFileInputStream::ReadBool()
		{
			bool value;

			inputFileStream.read(reinterpret_cast<char*>(&value), sizeof(value));

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			return value;
		}

		char* ZFileInputStream::ReadChars(const size_t size)
		{
			char* buffer = new char[size + 1];

			inputFileStream.read(buffer, size);

			if (endian == Endian::big)
			{
				SwapEndian(buffer, size);
			}

			return buffer;
		}

		std::string ZFileInputStream::ReadString()
		{
			std::string result;

			std::getline(inputFileStream, result);

			/*if (endian == Endian::big)
			{
				SwapEndian(result.data(), result.length());
			}*/

			return result;
		}

		std::string ZFileInputStream::ReadString(const size_t size)
		{
			char* buffer = ReadChars(size + 1);

			/*if (endian == Endian::big)
			{
				SwapEndian(buffer, size);
			}*/

			std::string result = std::string(buffer, size);

			delete[] buffer;

			return result;
		}

		void* ZFileInputStream::ReadBytes(const size_t size)
		{
			void* buffer = operator new(size);

			inputFileStream.read(static_cast<char*>(buffer), size);

			if (endian == Endian::big)
			{
				SwapEndian(buffer, size);
			}

			return buffer;
		}

		void ZFileInputStream::ReadBytes(void* buffer, const size_t size)
		{
			inputFileStream.read(static_cast<char*>(buffer), size);

			/*if (endian == Endian::big)
			{
				SwapEndian(buffer, size);
			}*/
		}

		IOI::SVector4 ZFileInputStream::ReadVector4()
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

		void ZFileInputStream::SwapEndian(void* buffer, size_t size)
		{
			unsigned char* buffer2 = static_cast<unsigned char*>(buffer);

			std::reverse(buffer2, buffer2 + size);
		}

		void ZFileInputStream::SetEndian(const Endian endian)
		{
			this->endian = endian;
		}
	}
}
