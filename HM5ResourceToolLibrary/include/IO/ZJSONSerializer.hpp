#pragma once

#include <iostream>
#include <string>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "IO/ZJSONDeserializer.hpp"
#include "Registry/ZEnumRegistry.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZJSONSerializer
		{
		public:
			ZJSONSerializer();
			bool Serialize(const std::string& typeName, void* data, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

		private:
			ZEnumRegistry enumRegistry;
		};
	}
}
