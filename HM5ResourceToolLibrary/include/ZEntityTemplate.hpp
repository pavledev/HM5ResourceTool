#pragma once

#include <string>
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "Glacier2/Entity/STemplateEntity.hpp"
#include "Glacier2/Entity/STemplateEntityBlueprint.hpp"
#include "Glacier2/Entity/SCppEntity.hpp"
#include "Glacier2/Entity/SCppEntityBlueprint.hpp"
#include "ZResource.hpp"
#include "SEntityTemplate.hpp"
#include "ZHeaderLibrary.hpp"
#include "MapNode.hpp"

namespace HM5ResourceTool
{
    class ZHeaderLibrary;

    class ZEntityTemplate
    {
    public:
        __declspec(dllexport) ZEntityTemplate();
        __declspec(dllexport) ~ZEntityTemplate();
        __declspec(dllexport) static bool ConvertTemplateEntityToJSON(const std::vector<ZResource*>* references, const std::string& templateEntityPath, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertTemplateEntityToJSON(const std::vector<ZResource*>* references, const void* data, const unsigned int dataSize, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertTemplateEntityBlueprintToJSON(const std::string& templateEntityBlueprintPath, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertTemplateEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertCppEntityToJSON(const std::vector<ZResource*>* references, const std::string& cppEntityPath, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertCppEntityToJSON(const std::vector<ZResource*>* references, const void* data, const unsigned int dataSize, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertCppEntityBlueprintToJSON(const std::string& cppEntityBlueprintPath, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertCppEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
        __declspec(dllexport) static bool ConvertJSONToTemplateEntity(const std::string& jsonPath, const std::vector<ZResource*>* references, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertJSONToTemplateEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertJSONToCppEntity(const std::string& jsonPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertJSONToCppEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertTEMPAndTBLUToJSON(const ZResource* resource, std::string& jsonOutput);
        __declspec(dllexport) SEntityTemplate* JoinTEMPAndTBLU(const ZResource* resource, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary);
        static void AddChildren(IOI::STemplateEntity* templateEntity, IOI::STemplateEntityBlueprint* templateEntityBlueprint, SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const ZResource* resource);
        static ZResource* GetTemplateEntityBlueprintReference(const ZResource* resource, int blueprintIndexInResourceHeader);
        __declspec(dllexport) MapNode* GetGeometryMap(const ZResource* resource, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary);
        void AddChildren(MapNode* parentNode, std::vector<MapNode*>& geometryEntities);
        ZResource* GetPrimReference(ZResource* reference, ZHeaderLibrary* headerLibrary, ZHeaderLibrary* globalHeaderLibrary);
        IOI::ZRuntimeResourceID GetPrimRuntimeResourceID(const ZResource* reference);
        void ExportMapModel(ZResource* reference, const std::string& objFilePath);

    private:
        IOI::STemplateEntity* templateEntity;
        IOI::STemplateEntityBlueprint* templateEntityBlueprint;
        unsigned char templateEntityAlignment;
        unsigned char templateEntityBlueprintAlignment;
        inline static ZPropertyRegistry propertyRegistry;
    };
}
