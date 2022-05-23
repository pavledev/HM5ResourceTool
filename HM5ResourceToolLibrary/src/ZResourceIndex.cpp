#include "pch.h"
#include "ZResourceIndex.hpp"
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "Glacier2/SResourceIndex.hpp"

namespace HM5ResourceTool
{
	bool ZResourceIndex::ConvertResourceIndexToJSON(const std::string& resourceIndexPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SResourceIndex* resourceIndex = static_cast<IOI::SResourceIndex*>(binaryDeserializer.Deserialize(resourceIndexPath));

			jsonOutput = resourceIndex->SerializeToJSON();
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

	bool ZResourceIndex::ConvertResourceIndexToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SResourceIndex* resourceIndex = static_cast<IOI::SResourceIndex*>(binaryDeserializer.Deserialize(data, dataSize));

			jsonOutput = resourceIndex->SerializeToJSON();
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
