#pragma once

#include "SPackageListDataFolder.hpp"

namespace IOI
{
	struct SPackageListData
	{
		TArray<SPackageListDataFolder> folders;

		std::string SerializeToJSON();
	};
}
