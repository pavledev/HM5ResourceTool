#pragma once

#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/Glacier2/Resource/SResourceHeaderHeader.hpp"

namespace HM5ResourceToolCLI
{
	public ref struct SResourceHeaderHeader
	{
		SResourceHeaderHeader();
		SResourceHeaderHeader(IOI::SResourceHeaderHeader* resourceHeaderHeader);

		IOI::SResourceHeaderHeader* instance;
	};
}
