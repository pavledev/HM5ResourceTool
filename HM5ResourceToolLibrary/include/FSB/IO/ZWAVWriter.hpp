#pragma once

#include <cstring>
#include "SWAVHeader.hpp"
#include "IMAADPCMWAVEFORMAT.hpp"
#include "../FSB.hpp"
#include "IO/ZBinaryWriter.hpp"

namespace FSB
{
	namespace IO
	{
		class ZWAVWriter
		{
		public:
			static SWAVHeader GenerateWAVHeader(const FSB::Sample& sample);
			static void WriteToFile(const FSB::Sample& sample, const char* const data, std::ostream& stream);
			static void WriteToFile(const FSB::Sample& sample, char* data, HM5ResourceTool::IO::ZBinaryWriter& binaryWriter);
		};
	}
}
