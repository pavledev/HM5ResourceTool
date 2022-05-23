#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZVideo.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZVideo : public ManagedObject<HM5ResourceTool::ZVideo>
    {
    public:
        static bool ConvertPCBinkVidToBINK(String^ pcBinkVidPath, [Out] array<Byte>^% binkData, [Out] unsigned int% binkSize);
    };
}
