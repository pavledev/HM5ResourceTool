#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZPackageList.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZPackageList : public ManagedObject<HM5ResourceTool::ZPackageList>
    {
    public:
        static bool ConvertPackgeListToJSON(String^ packageListPath, [Out] String^% jsonOutput);
    };
}
