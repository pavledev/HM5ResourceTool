#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZBehaviorTree.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZBehaviorTree : public ManagedObject<HM5ResourceTool::ZBehaviorTree>
    {
    public:
        ZBehaviorTree();
        static bool ConvertBehaviorTreeEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        //static bool ConvertJSONToBehaviorTreeEntityBlueprint(String^ jsonPath);
    };
}
