#pragma once

#include <string>
#include <vector>
#include "Glacier2/Math/SMatrix43.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "ZResource.hpp"

namespace HM5ResourceTool
{
	struct MapNode
	{
		int index;
		int eidParent;
		std::string name;
		bool hasTransform;
		bool hasEIDParent;
		IOI::SMatrix43 transform;
		bool visible;
		bool hasVisibleProperty;
		IOI::SVector3 globalSize;
		bool hasGlobalSize;
		int entityTypeResourceIndex;
		std::vector<MapNode*> children;
		bool hasResourceID;
		IOI::ZRuntimeResourceID primRuntimeResourceID;
		ZResource* primReference;

		MapNode()
		{
			index = 0;
			eidParent = 0;
			hasTransform = false;
			hasEIDParent = false;
			visible = false;
			hasGlobalSize = false;
			hasResourceID = false;
			primReference = nullptr;
			//primRuntimeResourceID.SetHigh(0);
			//primRuntimeResourceID.SetLow(0);
		}
	};
}
