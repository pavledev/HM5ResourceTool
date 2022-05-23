#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZChartType.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZChartType : public ManagedObject<HM5ResourceTool::ZChartType>
    {
    public:
        static bool ConvertChartTypeToText(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% output);
    };
}
