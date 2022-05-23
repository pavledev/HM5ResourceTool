#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZResourceIndex.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZResourceIndex : public ManagedObject<HM5ResourceTool::ZResourceIndex>
    {
    public:
        static bool ConvertResourceIndexToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% output);
    };
}
