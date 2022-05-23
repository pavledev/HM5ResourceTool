#include "pch.h"
#include "FSB/IO/ZBufferView.hpp"

namespace FSB
{
	namespace IO
	{
		ZBufferView::ZBufferView(const char* buffer, std::size_t length)
			: begin(buffer)
			, end(buffer + length)
			, current(buffer)
		{
		}

		ZBufferView::ZBufferView(const char* begin, const char* end)
			: begin(begin)
			, end(end)
			, current(begin)
		{
		}

		const char* ZBufferView::Begin() const
		{
			return begin;
		}

		const char* ZBufferView::End() const
		{
			return end;
		}

		const char* ZBufferView::Current() const
		{
			return current;
		}

		bool ZBufferView::Empty() const
		{
			return end == begin;
		}

		std::size_t ZBufferView::Size() const
		{
			return end - begin;
		}

		std::size_t ZBufferView::Remaining() const
		{
			return end - current;
		}

		std::size_t ZBufferView::Offset() const
		{
			return current - begin;
		}

		void ZBufferView::SetOffset(std::size_t offset)
		{
			if (offset > Size())
			{
				throw std::invalid_argument("Offset is greater then size.");
			}

			current = begin + offset;
		}

		void ZBufferView::Skip(std::size_t length)
		{
			if (Remaining() < length)
			{
				throw std::invalid_argument("Remaining size is smaller then length.");
			}

			current += length;
		}

		const char* ZBufferView::Read(std::size_t length)
		{
			if (Remaining() < length)
			{
				throw std::invalid_argument("Remaining size is smaller then length.");
			}

			const char* const result = current;
			current += length;
			return result;
		}

		char ZBufferView::ReadChar()
		{
			if (Remaining() < 1u)
			{
				throw std::invalid_argument("Remaining size is smaller then 1.");
			}

			return *current++;
		}

		uint8_t ZBufferView::ReadUInt8()
		{
			if (Remaining() < 1u)
			{
				throw std::invalid_argument("Remaining size is smaller then 1.");
			}

			return *current++;
		}

		uint16_t ZBufferView::ReadUInt16()
		{
			if (Remaining() < 1u)
			{
				throw std::invalid_argument("Remaining size is smaller then 2.");
			}

			const uint16_t x = ReadUInt8();
			const uint16_t y = ReadUInt8();
			return x + (y << 8u);
		}

		uint32_t ZBufferView::ReadUInt24()
		{
			if (Remaining() < 3u)
			{
				throw std::invalid_argument("Remaining size is smaller then 3.");
			}

			const uint32_t x = ReadUInt8();
			const uint32_t y = ReadUInt16();
			return x + (y << 8u);
		}

		uint32_t ZBufferView::ReadUInt32()
		{
			if (Remaining() < 4u)
			{
				throw std::invalid_argument("Remaining size is smaller then 4.");
			}

			const uint32_t x = ReadUInt16();
			const uint32_t y = ReadUInt16();
			return x + (y << 16u);
		}

		uint64_t ZBufferView::ReadUInt64()
		{
			if (Remaining() < 8u)
			{
				throw std::invalid_argument("Remaining size is smaller then 8.");
			}

			const uint64_t x = ReadUInt32();
			const uint64_t y = ReadUInt32();
			return x + (y << 32u);
		}

	}
}
