#include "pch.h"
#include "ZResource.hpp"

namespace HM5ResourceToolCLI
{
    ZResource::ZResource(HM5ResourceTool::ZResource* resource)
    {
        instance = resource;
    }

    const String^ ZResource::GetName()
    {
        return marshal_as<String^>(instance->GetName());
    }

    SResourceHeaderHeader^ ZResource::GetResourceHeaderHeader()
    {
        return gcnew SResourceHeaderHeader(instance->GetResourceHeaderHeader());
    }

    ZRuntimeResourceID^ ZResource::GetRuntimeResourceID()
    {
        return gcnew ZRuntimeResourceID(instance->GetRuntimeResourceID());
    }

    String^ ZResource::GetResourceID()
    {
        return marshal_as<String^>(instance->GetResourceID());
    }

    EResourceReferenceFlags ZResource::GetResourceReferenceFlags()
    {
        return static_cast<EResourceReferenceFlags>(instance->GetResourceReferenceFlags());
    }

    String^ ZResource::GetFileFormat()
    {
        return marshal_as<String^>(instance->GetFileFormat());
    }

    const unsigned int ZResource::GetOffsetInHeaderLibrary()
    {
        return instance->GetOffsetInHeaderLibrary();
    }

    const unsigned int ZResource::GetOffsetInResourceLibrary()
    {
        return instance->GetOffsetInResourceLibrary();
    }

    array<Byte>^ ZResource::GetHeaderData()
    {
        IntPtr headerDataPointer = IntPtr(instance->GetHeaderData());
        unsigned int size = instance->GetHeaderDataSize();
        array<Byte>^ bytes = gcnew array<Byte>(size);

        Marshal::Copy(headerDataPointer, bytes, 0, size);

        return bytes;
    }

    const unsigned int ZResource::GetHeaderDataSize()
    {
        return instance->GetHeaderDataSize();
    }

    array<Byte>^ ZResource::GetResourceData()
    {
        IntPtr headerDataPointer = IntPtr(instance->GetResourceData());
        unsigned int size = instance->GetResourceDataSize();
        array<Byte>^ bytes = gcnew array<Byte>(size);

        Marshal::Copy(headerDataPointer, bytes, 0, size);

        return bytes;
    }

    const unsigned int ZResource::GetResourceDataSize()
    {
        return instance->GetResourceDataSize();
    }

    List<ZResource^>^ ZResource::GetReferences()
    {
        std::vector<HM5ResourceTool::ZResource*>* references = instance->GetReferences();
        unsigned int size = references->size();
        List<ZResource^>^ result = gcnew List<ZResource^>(size);

        for (unsigned int i = 0; i < size; i++)
        {
            ZResource^ resource = gcnew ZResource(references->operator[](i));

            result->Add(resource);
        }

        return result;
    }

    const String^ ZResource::GetResourceDetails()
    {
        return marshal_as<String^>(instance->GetResourceDetails());
    }

    const bool ZResource::IsResourceDataLoaded()
    {
        return instance->IsResourceDataLoaded();
    }

    const String^ ZResource::GetHexBytesFromHeaderLibrary()
    {
        return marshal_as<String^>(instance->GetHexBytesFromHeaderLibrary());
    }

    const String^ ZResource::GetHexBytesFromResourceLibrary()
    {
        return marshal_as<String^>(instance->GetHexBytesFromResourceLibrary());
    }

    String^ ZResource::SerializeToJSON()
    {
        return marshal_as<String^>(instance->SerializeToJSON());
    }

    HM5ResourceTool::ZResource* ZResource::GetInstance()
    {
        return instance;
    }
}
