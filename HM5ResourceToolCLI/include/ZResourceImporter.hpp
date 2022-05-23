#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZResourceImporter.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZResourceImporter : public ManagedObject<HM5ResourceTool::ZResourceImporter>
    {
    public:
        ZResourceImporter(String^ metaDataFilePath, String^ gamePath);
        bool ImportResource(String^ inputFilePath, bool backupResourceLibrary);
    };
}
