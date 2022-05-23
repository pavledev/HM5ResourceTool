#pragma once

#include <fstream>
#include "IOutputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZMemoryOuputStream : public IOutputStream
		{
		public:
			ZMemoryOuputStream();
			ZMemoryOuputStream(const size_t capacity, Endian endian = Endian::little);
			ZMemoryOuputStream(const void* buffer, size_t capacity, Endian endian = Endian::little);
			~ZMemoryOuputStream() override;
			size_t GetCapacity() const override;
			size_t GetPosition() override;
			void* GetBuffer() const override;
			void* GetCurrentPointer() const override;
			void Close() override;
			void Skip(const size_t count) override;
			void Seek(const size_t offset, const ESeekOrigin seekOrigin) override;
			void WriteChar(char value) override;
			void WriteByte(unsigned char value) override;
			void WriteInt16(short value) override;
			void WriteInt32(int value) override;
			void WriteInt64(long long value) override;
			void WriteUInt16(unsigned short value) override;
			void WriteUInt32(unsigned int value) override;
			void WriteUInt64(unsigned long long value) override;
			void WriteFloat(float value) override;
			void WriteDouble(double value) override;
			void WriteBool(bool value) override;
			void WriteString(std::string string) override;
			void WriteBytes(void* buffer, const size_t size) override;
			void EnsureCapacity(const size_t size);
			void AlignTo(size_t alignment) override;
			void SwapEndian(void* buffer, size_t size) override;
			void SetEndian(const Endian endian) override;

		private:
			void* buffer;
			size_t capacity;
			size_t position;
			bool isExternalBuffer;
			Endian endian;
		};
	}
}
