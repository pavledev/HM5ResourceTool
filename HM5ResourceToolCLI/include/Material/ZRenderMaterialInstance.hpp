#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/Material/ZRenderMaterialInstance.hpp"
//#include "SRMaterialProperties.h"

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZRenderMaterialInstance : public ManagedObject<HM5ResourceTool::ZRenderMaterialInstance>
    {
    public:
        ref struct SProperty
        {
        public:
            SProperty();
            SProperty(HM5ResourceTool::ZRenderMaterialInstance::SProperty* property);
            String^ GetName();
            unsigned int GetData();
            unsigned int GetSize();
            unsigned int GetType();

        private:
            HM5ResourceTool::ZRenderMaterialInstance::SProperty* property;
        };

        ref class ZProperty
        {
        public:
            ZProperty();
            ZProperty(HM5ResourceTool::ZRenderMaterialInstance::ZProperty* property);
            SProperty^ GetPropertyInfo();
            array<Byte>^ GetValue();
            List<ZRenderMaterialInstance::ZProperty^>^ GetChildProperties();

        private:
            HM5ResourceTool::ZRenderMaterialInstance::ZProperty* property;
        };

        ZRenderMaterialInstance();
        //SRMaterialProperties^ GetMaterialInfo();
        ZRenderMaterialInstance::ZProperty^ GetInstanceProperty();
        void Deserialize(String^ filePath);
        void Deserialize(const array<Byte>^ data, const unsigned int dataSize);
    };
}
