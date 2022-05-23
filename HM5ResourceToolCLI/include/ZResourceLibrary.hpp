#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZResourceLibrary.hpp"
#include "ZResource.hpp"

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
	public ref class ZResourceLibrary
	{
	public:
		ZResourceLibrary(HM5ResourceTool::ZResourceLibrary* resourceLibrary);
		void LoadResourceData(const unsigned int resourceIndex);
		String^ GetResourceID();
		String^ GetFilePath();
		ZRuntimeResourceID^ GetRuntimeResourceID();
		SResourceHeaderHeader^ GetResourceHeaderHeader();
		List<ZResource^>^ GetResources();

	private:
		HM5ResourceTool::ZResourceLibrary* instance;
	};
}
