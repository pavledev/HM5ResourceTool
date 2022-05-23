#include "pch.h"
#include <cassert>
#include "IO/ZMemoryOuputStream.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZMemoryOuputStream::ZMemoryOuputStream()
		{
			this->buffer = calloc(1, 256);
			this->capacity = 256;
			this->position = 0;
			this->isExternalBuffer = false;
			this->endian = Endian::little;
		}

		ZMemoryOuputStream::ZMemoryOuputStream(const size_t capacity, Endian endian)
		{
			this->buffer = calloc(1, capacity);
			this->capacity = capacity;
			this->position = 0;
			this->isExternalBuffer = false;
			this->endian = endian;
		}

		ZMemoryOuputStream::ZMemoryOuputStream(const void* buffer, size_t capacity, Endian endian)
		{
			this->buffer = const_cast<void*>(buffer);
			this->capacity = capacity;
			this->position = 0;
			this->isExternalBuffer = true;
			this->endian = endian;
		}

		ZMemoryOuputStream::~ZMemoryOuputStream()
		{
			if (!isExternalBuffer)
			{
				delete[] buffer;
			}
		}

		size_t ZMemoryOuputStream::GetCapacity() const
		{
			return capacity;
		}

		size_t ZMemoryOuputStream::GetPosition()
		{
			return position;
		}

		void* ZMemoryOuputStream::GetBuffer() const
		{
			return buffer;
		}

		void* ZMemoryOuputStream::GetCurrentPointer() const
		{
			return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + position);
		}

		void ZMemoryOuputStream::Close()
		{
			if (!isExternalBuffer)
			{
				delete[] buffer;
			}
		}

		void ZMemoryOuputStream::Skip(const size_t count)
		{
			Seek(count, ESeekOrigin::Current);
		}

		void ZMemoryOuputStream::Seek(const size_t offset, const ESeekOrigin seekOrigin)
		{
			switch (seekOrigin)
			{
			case ESeekOrigin::Begin:
			{
				assert(position <= capacity);

				position = offset;

				break;
			}
			case ESeekOrigin::Current:
			{
				assert(position + offset <= capacity);

				position += offset;

				break;
			}
			case ESeekOrigin::End:
			{
				assert(position + offset >= 0);

				position = capacity - offset;

				break;
			}
			}
		}

		void ZMemoryOuputStream::WriteChar(char value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<char*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteByte(unsigned char value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<unsigned char*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteInt16(short value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<short*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteInt32(int value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<int*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteInt64(long long value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<long long*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteUInt16(unsigned short value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<unsigned short*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteUInt32(unsigned int value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			/*if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}*/

			*reinterpret_cast<unsigned int*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteUInt64(unsigned long long value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<unsigned long long*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteFloat(float value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<float*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteDouble(double value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<double*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteBool(bool value)
		{
			EnsureCapacity(sizeof(value));

			uintptr_t pointer = reinterpret_cast<uintptr_t>(buffer) + position;

			if (endian == Endian::big)
			{
				SwapEndian(&value, sizeof(value));
			}

			*reinterpret_cast<bool*>(pointer) = value;

			position += sizeof(value);
		}

		void ZMemoryOuputStream::WriteString(std::string string)
		{
			EnsureCapacity(string.length());

			if (endian == Endian::big)
			{
				SwapEndian(string.data(), string.length());
			}

			WriteBytes(string.data(), string.length());

			WriteChar(0);
		}

		void ZMemoryOuputStream::WriteBytes(void* buffer, const size_t size)
		{
			EnsureCapacity(size);

			if (endian == Endian::big)
			{
				SwapEndian(buffer, size);
			}

			memcpy(GetCurrentPointer(), buffer, size);

			position += size;
		}

		void ZMemoryOuputStream::EnsureCapacity(const size_t size)
		{
			if (position + size > capacity)
			{
				size_t newCapacity = capacity * 2;

				while (newCapacity < size)
				{
					newCapacity *= 2;
				}

				buffer = realloc(buffer, newCapacity);

				size_t size2 = newCapacity - capacity;
				void* buffer2 = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + capacity);

				memset(buffer2, 0, size2);

				capacity = newCapacity;
			}
		}

		void ZMemoryOuputStream::AlignTo(size_t alignment)
		{
			if (position % alignment != 0)
			{
				const unsigned int numberOfBytesToSkip = alignment - position % alignment;

				EnsureCapacity(position + numberOfBytesToSkip);

				memset(GetCurrentPointer(), 0, numberOfBytesToSkip);

				position += numberOfBytesToSkip;
			}
		}

		void ZMemoryOuputStream::SwapEndian(void* buffer, size_t size)
		{
			unsigned char* buffer2 = static_cast<unsigned char*>(buffer);

			std::reverse(buffer2, buffer2 + size);
		}

		void ZMemoryOuputStream::SetEndian(const Endian endian)
		{
			this->endian = endian;
		}
	}
}
