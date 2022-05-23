#include "pch.h"
#include "ZEntityTemplate.hpp"

namespace HM5ResourceToolCLI
{
	ZEntityTemplate::ZEntityTemplate() : ManagedObject(new HM5ResourceTool::ZEntityTemplate())
	{

	}

	bool ZEntityTemplate::ConvertTemplateEntityToJSON(ZResource^ resource, [Out] String^% jsonOutput)
	{
		std::vector<HM5ResourceTool::ZResource*>* references = resource->GetInstance()->GetReferences();
		void* data = resource->GetInstance()->GetResourceData();
		unsigned int resourceDataSize = resource->GetInstance()->GetResourceDataSize();
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZEntityTemplate::ConvertTemplateEntityToJSON(references, data, resourceDataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZEntityTemplate::ConvertCppEntityToJSON(ZResource^ resource, [Out] String^% jsonOutput)
	{
		std::vector<HM5ResourceTool::ZResource*>* references = resource->GetInstance()->GetReferences();
		void* data = resource->GetInstance()->GetResourceData();
		unsigned int resourceDataSize = resource->GetInstance()->GetResourceDataSize();
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZEntityTemplate::ConvertCppEntityToJSON(references, data, resourceDataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZEntityTemplate::ConvertCppEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZEntityTemplate::ConvertCppEntityBlueprintToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	/*bool ZEntityTemplate::ConvertJSONToTemplateEntity(String^ jsonPath)
	{
		std::string jsonPath2 = marshal_as<std::string>(jsonPath);

		return HM5ResourceTool::ZEntityTemplate::ConvertJSONToTemplateEntity(jsonPath2);
	}

	bool ZEntityTemplate::ConvertJSONToTemplateEntityBlueprint(String^ jsonPath)
	{
		std::string jsonPath2 = marshal_as<std::string>(jsonPath);

		return HM5ResourceTool::ZEntityTemplate::ConvertJSONToTemplateEntityBlueprint(jsonPath2);
	}

	bool ZEntityTemplate::ConvertJSONToCppEntity(String^ jsonPath)
	{
		std::string jsonPath2 = marshal_as<std::string>(jsonPath);

		return HM5ResourceTool::ZEntityTemplate::ConvertJSONToCppEntity(jsonPath2);
	}

	bool ZEntityTemplate::ConvertJSONToCppEntityBlueprint(String^ jsonPath)
	{
		std::string jsonPath2 = marshal_as<std::string>(jsonPath);

		return HM5ResourceTool::ZEntityTemplate::ConvertJSONToCppEntityBlueprint(jsonPath2);
	}*/

	bool ZEntityTemplate::ConvertTEMPAndTBLUToJSON(ZResource^ resource, [Out] String^% jsonOutput)
	{
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZEntityTemplate::ConvertTEMPAndTBLUToJSON(resource->GetInstance(), jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	SEntityTemplate^ ZEntityTemplate::JoinTEMPAndTBLU(ZResource^ resource, ZHeaderLibrary^ headerLibrary, ZHeaderLibrary^ globalHeaderLibrary)
	{
		return gcnew SEntityTemplate(instance->JoinTEMPAndTBLU(resource->GetInstance(), headerLibrary->GetInstance(), globalHeaderLibrary->GetInstance()));
	}

	MapNode^ ZEntityTemplate::GetGeometryMap(ZResource^ resource, ZHeaderLibrary^ headerLibrary, ZHeaderLibrary^ globalHeaderLibrary)
	{
		return gcnew MapNode(instance->GetGeometryMap(resource->GetInstance(), headerLibrary->GetInstance(), globalHeaderLibrary->GetInstance()));
	}
}
