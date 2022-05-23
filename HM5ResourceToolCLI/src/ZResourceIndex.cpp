#include "pch.h"
#include "ZResourceIndex.hpp"

namespace HM5ResourceToolCLI
{
	bool ZResourceIndex::ConvertResourceIndexToJSON(const array<Byte>^ data, const unsigned int dataSize, String^% output)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string output2;

		bool result = HM5ResourceTool::ZResourceIndex::ConvertResourceIndexToJSON(dataPointer, dataSize, output2);

		output = gcnew String(output2.c_str());

		return result;
	}
}
