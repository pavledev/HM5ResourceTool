#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZModel.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZModel : public ManagedObject<HM5ResourceTool::ZModel>
    {
    public:
        ZModel();
        static bool ConvertRenderPrimitiveToOBJ(const array<Byte>^ data, const unsigned int dataSize, String^ objFilePath);
        static bool ConvertRenderPrimitiveToGLTF(const array<Byte>^ data, const unsigned int dataSize, String^ objFilePath);
    };
}
