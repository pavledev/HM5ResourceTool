#include "pch.h"
#include "ZMaterial.hpp"

namespace HM5ResourceToolCLI
{
	ZMaterial::ZMaterial() : ManagedObject(new HM5ResourceTool::ZMaterial())
	{

	}

	bool ZMaterial::ConvertRenderMaterialEntityToJSON(const array<Byte>^ data, const unsigned int dataSize, String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZMaterial::ConvertRenderMaterialEntityToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZMaterial::ConvertRenderMaterialEntityAndBlueprintToJSON(ZResource^ mattResource, [Out] String^% jsonOutput)
	{
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZMaterial::ConvertRenderMaterialEntityAndBlueprintToJSON(mattResource->GetInstance(), jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}
}
