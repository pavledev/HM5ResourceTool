#pragma once

#include <fstream>
#include "IInputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZFileInputStream : public IInputStream
		{
		public:
			ZFileInputStream();
			ZFileInputStream(const std::string& filePath, Endian endian = Endian::little);
			~ZFileInputStream() override;
			size_t GetSize() const override;
			size_t GetPosition() override;
			void* GetBuffer() const override;
			void* GetCurrentPointer() const override;
			void Close() override;
			void Skip(const size_t count) override;
			void Seek(const size_t offset, const ESeekOrigin seekOrigin) override;
			char ReadChar() override;
			unsigned char ReadByte() override;
			short ReadInt16() override;
			int ReadInt32() override;
			long long ReadInt64() override;
			unsigned short ReadUInt16() override;
			unsigned int ReadUInt32() override;
			unsigned long long ReadUInt64() override;
			float ReadFloat() override;
			double ReadDouble() override;
			bool ReadBool() override;
			char* ReadChars(const size_t size) override;
			std::string ReadString() override;
			std::string ReadString(const size_t size) override;
			void* ReadBytes(const size_t size) override;
			void ReadBytes(void* buffer, const size_t size) override;
			IOI::SVector4 ReadVector4() override;
			void SwapEndian(void* buffer, size_t size) override;
			void SetEndian(const Endian endian) override;

		private:
			std::ifstream inputFileStream;
			size_t size;
			Endian endian;
		};
	}
}
