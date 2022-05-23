#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/SAudioSample.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
	public ref struct SAudioSample : public ManagedObject<HM5ResourceTool::SAudioSample>
	{
		//array<Byte>^ data;
		//unsigned int dataSize;
		//String^ name;

		SAudioSample();
		SAudioSample(HM5ResourceTool::SAudioSample* audioSample);
		array<Byte>^ GetData();
		unsigned int GetDataSize();
		String^ GetName();
	};
}
