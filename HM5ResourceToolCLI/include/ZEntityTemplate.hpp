#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZEntityTemplate.hpp"
#include "SEntityTemplate.hpp"
#include "ZHeaderLibrary.hpp"
#include "MapNode.hpp"

using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZEntityTemplate : public ManagedObject<HM5ResourceTool::ZEntityTemplate>
    {
    public:
        ZEntityTemplate();
        static bool ConvertTemplateEntityToJSON(ZResource^ resource, [Out] String^% jsonOutput);
        static bool ConvertTemplateEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        static bool ConvertCppEntityToJSON(ZResource^ resource, [Out] String^% jsonOutput);
        static bool ConvertCppEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        /*static bool ConvertJSONToTemplateEntity(String^ jsonPath);
        static bool ConvertJSONToTemplateEntityBlueprint(String^ jsonPath);
        static bool ConvertJSONToCppEntity(String^ jsonPath);
        static bool ConvertJSONToCppEntityBlueprint(String^ jsonPath);*/
        static bool ConvertTEMPAndTBLUToJSON(ZResource^ resource, [Out] String^% jsonOutput);
        SEntityTemplate^ JoinTEMPAndTBLU(ZResource^ resource, ZHeaderLibrary^ headerLibrary, ZHeaderLibrary^ globalHeaderLibrary);
        MapNode^ GetGeometryMap(ZResource^ resource, ZHeaderLibrary^ headerLibrary, ZHeaderLibrary^ globalHeaderLibrary);
    };
}
