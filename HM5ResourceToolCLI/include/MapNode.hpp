#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/MapNode.hpp"

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
	public ref struct MapNode : public ManagedObject<HM5ResourceTool::MapNode>
	{
		MapNode(HM5ResourceTool::MapNode* mapNode);
	};
}
