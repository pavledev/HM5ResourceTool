#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZMaterial.hpp"
#include "ZResource.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZMaterial : public ManagedObject<HM5ResourceTool::ZMaterial>
    {
    public:
        ZMaterial();
        static bool ConvertRenderMaterialEntityToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        static bool ConvertRenderMaterialEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        static bool ConvertRenderMaterialEntityAndBlueprintToJSON(ZResource^ mattResource, [Out] String^% jsonOutput);
    };
}
