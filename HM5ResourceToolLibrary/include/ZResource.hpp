#pragma once

#include <vector>
#include "Glacier2/Resource/SResourceHeaderHeader.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/Resource/EResourceReferenceFlags.hpp"

namespace HM5ResourceTool
{
    class ZResource
    {
    public:
        __declspec(dllexport) ZResource();
        __declspec(dllexport) ~ZResource();
        const std::string GetHeaderLibraryPath() const;
        const std::string GetResourceLibraryPath() const;
        const std::string GetName() const;
        __declspec(dllexport) const std::string GetName();
        __declspec(dllexport) IOI::SResourceHeaderHeader* GetResourceHeaderHeader();
        const IOI::SResourceHeaderHeader* GetResourceHeaderHeader() const;
        const IOI::ZRuntimeResourceID* GetRuntimeResourceID() const;
        __declspec(dllexport) IOI::ZRuntimeResourceID* GetRuntimeResourceID();
        __declspec(dllexport) std::string GetResourceID() const;
        const IOI::EResourceReferenceFlags GetResourceReferenceFlags() const;
        __declspec(dllexport) IOI::EResourceReferenceFlags GetResourceReferenceFlags();
        __declspec(dllexport) std::string GetFileFormat() const;
        __declspec(dllexport) const unsigned int GetOffsetInHeaderLibrary() const;
        __declspec(dllexport) const unsigned int GetOffsetInResourceLibrary() const;
        const void* GetHeaderData() const;
        __declspec(dllexport) void* GetHeaderData();
        __declspec(dllexport) const unsigned int GetHeaderDataSize() const;
        const void* GetResourceData() const;
        __declspec(dllexport) void* GetResourceData();
        __declspec(dllexport) const unsigned int GetResourceDataSize() const;
        __declspec(dllexport) std::vector<ZResource*>* GetReferences();
        const std::vector<ZResource*>* GetReferences() const;
        void SetHeaderLibraryPath(std::string headerLibraryPath);
        void SetResourceLibraryPath(std::string resourceLibraryPath);
        void SetName(std::string name);
        void SetResourceHeaderHeader(const IOI::SResourceHeaderHeader& resourceHeaderHeader);
        void SetRuntimeResourceID(const IOI::ZRuntimeResourceID& runtimeResourceID);
        void SetResourceID(std::string resourceID);
        void SetResourceReferenceFlags(const IOI::EResourceReferenceFlags resourceReferenceFlags);
        void SetOffsetInHeaderLibrary(const unsigned int offsetInHeaderLibrary);
        void SetOffsetInResourceLibrary(const unsigned int offsetInResourceLibrary);
        void SetHeaderData(void* headerData);
        void SetHeaderDataSize(const unsigned int headerDataSize);
        void SetResourceData(void* resourceData);
        void SetResourceDataSize(const unsigned int resourceDataSize);
        void AddReference(ZResource* reference);
        void SetReferences(std::vector<ZResource*> references);
        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
        std::string SerializeToJSON() const;
        __declspec(dllexport) std::string SerializeToJSON();
        static ZResource* DeserializeFromJSON(const rapidjson::Document& document);
        __declspec(dllexport) const std::string GetResourceDetails();
        __declspec(dllexport) const bool IsResourceDataLoaded();
        const bool IsResourceDataLoaded() const;
        __declspec(dllexport) const std::string GetHexBytesFromHeaderLibrary();
        __declspec(dllexport) const std::string GetHexBytesFromResourceLibrary();
        __declspec(dllexport) const std::string GetHexBytes(const unsigned char* data, unsigned int dataSize);

    private:
        std::string headerLibraryPath;
        std::string resourceLibraryPath;
        std::string name;
        IOI::SResourceHeaderHeader resourceHeaderHeader;
        IOI::ZRuntimeResourceID runtimeResourceID;
        std::string resourceID;
        IOI::EResourceReferenceFlags resourceReferenceFlags;
        unsigned int offsetInHeaderLibrary;
        unsigned int offsetInResourceLibrary;
        void* headerData;
        unsigned int headerDataSize;
        void* resourceData;
        unsigned int resourceDataSize;
        std::vector<ZResource*> references;
    };
}
