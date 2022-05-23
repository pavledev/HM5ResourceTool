#include "pch.h"
#include "Glacier2/SResourceIndexFolder.hpp"

namespace IOI
{
	void SResourceIndexFolder::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("name");
		name.SerializeToJSON(writer);

		writer.String("resourceIndices");
		resourceIndices.SerializeToJSON(writer);

		writer.EndObject();
	}
}
