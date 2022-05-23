#include "pch.h"
#include "IO/ZJSONDeserializer.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZJSONDeserializer::ZJSONDeserializer(const std::string& filePath)
		{
			stream = std::ifstream(filePath);
		}

		ZJSONDeserializer::~ZJSONDeserializer()
		{
			stream.close();
		}
	}
}
