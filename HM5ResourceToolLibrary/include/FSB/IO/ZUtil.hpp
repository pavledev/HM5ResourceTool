#pragma once

#include <cstddef>
#include <iosfwd>
#include <vector>
#include "IO/ZBinaryReader.hpp"

namespace FSB
{
	namespace IO
	{
		class ZUtility
		{
		public:
			// Reads exactly size bytes from a stream and writes them to buffer.
			static void Read(std::istream& stream, char* buffer, std::size_t size);

			// Reads exactly size bytes from a stream and returns them.
			static std::vector<char> Read(std::istream& stream, std::size_t size);

			static void Read(HM5ResourceTool::IO::ZBinaryReader& binaryReader, char* buffer, std::size_t size);

			static std::vector<char> Read(HM5ResourceTool::IO::ZBinaryReader& binaryReader, std::size_t size);
		};
	}
}
