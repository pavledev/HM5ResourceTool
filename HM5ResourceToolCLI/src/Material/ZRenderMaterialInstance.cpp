#include "pch.h"
#include <sstream>
#include "Material/ZRenderMaterialInstance.hpp"

namespace HM5ResourceToolCLI
{
    ZRenderMaterialInstance::ZRenderMaterialInstance() : ManagedObject(new HM5ResourceTool::ZRenderMaterialInstance())
    {

    }

    /*SRMaterialProperties^ ZRenderMaterialInstance::GetMaterialInfo()
    {
        return gcnew SRMaterialProperties(instance->GetMaterialInfo());
    }*/

    ZRenderMaterialInstance::ZProperty^ ZRenderMaterialInstance::GetInstanceProperty()
    {
        return gcnew ZRenderMaterialInstance::ZProperty(instance->GetInstanceProperty());
    }

    void ZRenderMaterialInstance::Deserialize(String^ filePath)
    {
        std::string filePath2 = marshal_as<std::string>(filePath);

        instance->Deserialize(filePath2);
    }

    void ZRenderMaterialInstance::Deserialize(const array<Byte>^ data, const unsigned int dataSize)
    {
        pin_ptr<System::Byte> dataPointer = &data[0];

        instance->Deserialize(dataPointer, dataSize);
    }

    ZRenderMaterialInstance::SProperty::SProperty()
    {

    }

    ZRenderMaterialInstance::SProperty::SProperty(HM5ResourceTool::ZRenderMaterialInstance::SProperty* property)
    {
        this->property = property;
    }

    String^ ZRenderMaterialInstance::SProperty::GetName()
    {
        std::stringstream stream;

        stream << std::hex << property->lName;

        std::string hex(stream.str());

        int length = hex.length();
        std::string result;

        for (int i = 0; i < length; i += 2)
        {
            std::string byte = hex.substr(i, 2);
            char chr = static_cast<char>(static_cast<int>(strtol(byte.c_str(), nullptr, 16)));

            result.push_back(chr);
        }

        return marshal_as<String^>(result);
    }

    unsigned int ZRenderMaterialInstance::SProperty::GetData()
    {
        return property->lData;
    }

    unsigned int ZRenderMaterialInstance::SProperty::GetSize()
    {
        return property->lSize;
    }

    unsigned int ZRenderMaterialInstance::SProperty::GetType()
    {
        return property->lType;
    }

    ZRenderMaterialInstance::ZProperty::ZProperty()
    {

    }

    ZRenderMaterialInstance::ZProperty::ZProperty(HM5ResourceTool::ZRenderMaterialInstance::ZProperty* property)
    {
        this->property = property;
    }

    ZRenderMaterialInstance::SProperty^ ZRenderMaterialInstance::ZProperty::GetPropertyInfo()
    {
        return gcnew ZRenderMaterialInstance::SProperty(property->GetPropertyInfo());
    }

    array<Byte>^ ZRenderMaterialInstance::ZProperty::GetValue()
    {
        void* value = property->GetValue();
        IntPtr valuePointer = IntPtr(value);
        array<Byte>^ result = gcnew array<Byte>(property->GetValueSize());

        Marshal::Copy(valuePointer, result, 0, property->GetValueSize());

        return result;
    }

    List<ZRenderMaterialInstance::ZProperty^>^ ZRenderMaterialInstance::ZProperty::GetChildProperties()
    {
        std::vector<HM5ResourceTool::ZRenderMaterialInstance::ZProperty*>* childProperties = property->GetChildProperties();
        List<ZRenderMaterialInstance::ZProperty^>^ result = gcnew List<ZRenderMaterialInstance::ZProperty^>();

        for (unsigned int i = 0; i < childProperties->size(); ++i)
        {
            ZRenderMaterialInstance::ZProperty^ property = gcnew ZRenderMaterialInstance::ZProperty(childProperties->operator[](i));

            result->Add(property);
        }

        return result;
    }
}
