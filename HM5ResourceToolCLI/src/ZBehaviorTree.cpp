#include "pch.h"
#include "ZBehaviorTree.hpp"

namespace HM5ResourceToolCLI
{
	ZBehaviorTree::ZBehaviorTree() : ManagedObject(new HM5ResourceTool::ZBehaviorTree())
	{

	}

	bool ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	/*bool ZBehaviorTree::ConvertJSONToBehaviorTreeEntityBlueprint(String^ jsonPath)
	{
		std::string jsonPath2 = marshal_as<std::string>(jsonPath);

		return HM5ResourceTool::ZBehaviorTree::ConvertJSONToBehaviorTreeEntityBlueprint(jsonPath2);
	}*/
}
