#include "pch.h"
#include "ZResourceImporter.hpp"

namespace HM5ResourceToolCLI
{
	ZResourceImporter::ZResourceImporter(String^ metaDataFilePath, String^ gamePath) : ManagedObject(new HM5ResourceTool::ZResourceImporter(marshal_as<std::string>(metaDataFilePath), marshal_as<std::string>(gamePath)))
	{

	}

	bool ZResourceImporter::ImportResource(String^ inputFilePath, bool backupResourceLibrary)
	{
		std::string inputFilePath2 = marshal_as<std::string>(inputFilePath);

		return instance->ImportResource(inputFilePath2, backupResourceLibrary);
	}
}
