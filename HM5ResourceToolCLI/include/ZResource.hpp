#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZResource.hpp"
#include "Glacier2/Resource/SResourceHeaderHeader.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/Resource/EResourceReferenceFlags.hpp"

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZResource
    {
    public:
        ZResource(HM5ResourceTool::ZResource* resource);
        const String^ GetName();
        SResourceHeaderHeader^ GetResourceHeaderHeader();
        ZRuntimeResourceID^ GetRuntimeResourceID();
        String^ GetResourceID();
        EResourceReferenceFlags GetResourceReferenceFlags();
        String^ GetFileFormat();
        const unsigned int GetOffsetInHeaderLibrary();
        const unsigned int GetOffsetInResourceLibrary();
        array<Byte>^ GetHeaderData();
        const unsigned int GetHeaderDataSize();
        array<Byte>^ GetResourceData();
        const unsigned int GetResourceDataSize();
        List<ZResource^>^ GetReferences();
        const String^ GetResourceDetails();
        const bool IsResourceDataLoaded();
        const String^ GetHexBytesFromHeaderLibrary();
        const String^ GetHexBytesFromResourceLibrary();
        String^ SerializeToJSON();
        HM5ResourceTool::ZResource* GetInstance();

    private:
        HM5ResourceTool::ZResource* instance;
    };
}
