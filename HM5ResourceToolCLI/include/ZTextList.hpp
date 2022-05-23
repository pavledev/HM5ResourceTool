#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZTextList.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZTextList : public ManagedObject<HM5ResourceTool::ZTextList>
    {
    public:
        static bool ConvertTextListToText(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% output);
    };
}
