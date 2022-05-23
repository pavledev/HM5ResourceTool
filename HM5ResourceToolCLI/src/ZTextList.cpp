#include "pch.h"
#include "ZTextList.hpp"

namespace HM5ResourceToolCLI
{
	bool ZTextList::ConvertTextListToText(const array<Byte>^ data, const unsigned int dataSize, String^% output)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string output2;

		bool result = HM5ResourceTool::ZTextList::ConvertTextListToText(dataPointer, dataSize, output2);

		output = gcnew String(output2.c_str());

		return result;
	}
}
