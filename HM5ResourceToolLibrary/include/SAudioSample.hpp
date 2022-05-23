#pragma once

#include <string>

namespace HM5ResourceTool
{
	struct SAudioSample
	{
		void* data;
		unsigned int dataSize;
		std::string name;
	};
}
