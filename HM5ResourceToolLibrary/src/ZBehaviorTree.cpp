#include "pch.h"
#include "ZBehaviorTree.hpp"

namespace HM5ResourceTool
{
	bool ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(const std::string& behaviorTreeEntityBlueprintPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SBehaviorTreeInfo* behaviorTreeInfo = static_cast<IOI::SBehaviorTreeInfo*>(binaryDeserializer.Deserialize(behaviorTreeEntityBlueprintPath));

			jsonOutput = behaviorTreeInfo->SerializeToJSON();

			operator delete(behaviorTreeInfo, std::align_val_t(binaryDeserializer.GetAlignment()));
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SBehaviorTreeInfo* behaviorTreeInfo = static_cast<IOI::SBehaviorTreeInfo*>(binaryDeserializer.Deserialize(data, dataSize));

			jsonOutput = behaviorTreeInfo->SerializeToJSON();

			operator delete(behaviorTreeInfo, std::align_val_t(binaryDeserializer.GetAlignment()));
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZBehaviorTree::ConvertJSONToBehaviorTreeEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize)
	{
		try
		{
			HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(jsonPath);
			IOI::SBehaviorTreeInfo* behaviorTreeInfo = jsonDeserializer.Deserialize<IOI::SBehaviorTreeInfo>();
			IOI::ZBinarySerializer binarySerializer;

			binarySerializer.SerializeToMemory(behaviorTreeInfo, data, dataSize);

			delete behaviorTreeInfo;
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}
}
