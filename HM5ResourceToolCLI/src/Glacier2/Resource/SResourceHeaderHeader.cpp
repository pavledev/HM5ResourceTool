#include "pch.h"
#include "Glacier2/Resource/SResourceHeaderHeader.hpp"

namespace HM5ResourceToolCLI
{
	SResourceHeaderHeader::SResourceHeaderHeader()
	{

	}

	SResourceHeaderHeader::SResourceHeaderHeader(IOI::SResourceHeaderHeader* resourceHeaderHeader)
	{
		this->instance = resourceHeaderHeader;
	}
}
