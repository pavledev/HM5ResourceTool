#include "pch.h"
#include <format>
#include <Windows.h>
#include "IO/ZJSONSerializer.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/Math/SMatrix43.hpp"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/TArray.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/ZString.hpp"
#include "Glacier2/ZCurve.hpp"

namespace HM5ResourceTool
{
	namespace IO
	{
		ZJSONSerializer::ZJSONSerializer()
		{
			char buffer[MAX_PATH];

			GetModuleFileNameA(NULL, buffer, MAX_PATH);

			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			std::string filePath = std::format("{}/Enums.json", std::string(buffer).substr(0, pos));
			ZJSONDeserializer jsonDeserializer = ZJSONDeserializer(filePath);

			jsonDeserializer.Deserialize<ZEnumRegistry>(enumRegistry);
		}

		bool ZJSONSerializer::Serialize(const std::string& typeName, void* data, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			if (typeName == "int8" || typeName == "char")
			{
				writer.Int(*static_cast<char*>(data));
			}
			else if (typeName == "uint8")
			{
				writer.Uint(*static_cast<unsigned char*>(data));
			}
			else if (typeName == "int16")
			{
				writer.Int(*static_cast<short*>(data));
			}
			else if (typeName == "uint16")
			{
				writer.Uint(*static_cast<unsigned short*>(data));
			}
			else if (typeName == "int32")
			{
				writer.Int(*static_cast<int*>(data));
			}
			else if (typeName == "uint32")
			{
				writer.Uint(*static_cast<unsigned int*>(data));
			}
			else if (typeName == "int64")
			{
				writer.Int64(*static_cast<long long*>(data));
			}
			else if (typeName == "uint64")
			{
				writer.Uint64(*static_cast<unsigned long long*>(data));
			}
			else if (typeName == "float32")
			{
				writer.Double(*static_cast<float*>(data));
			}
			else if (typeName == "float64")
			{
				writer.Double(*static_cast<double*>(data));
			}
			else if (typeName == "bool")
			{
				writer.Bool(*static_cast<bool*>(data));
			}
			else if (typeName == "SVector2")
			{
				static_cast<IOI::SVector2*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "SVector3")
			{
				static_cast<IOI::SVector3*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "SMatrix43")
			{
				static_cast<IOI::SMatrix43*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "SColorRGB")
			{
				static_cast<IOI::SColorRGB*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "SEntityTemplateReference")
			{
				static_cast<IOI::SEntityTemplateReference*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "ZRuntimeResourceID")
			{
				static_cast<IOI::ZRuntimeResourceID*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "TArray<SEntityTemplateReference>")
			{
				static_cast<IOI::TArray<IOI::SEntityTemplateReference>*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "TArray<float32>")
			{
				static_cast<IOI::TArray<float>*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "ZString")
			{
				static_cast<IOI::ZString*>(data)->SerializeToJSON(writer);
			}
			else if (typeName == "ZCurve")
			{
				static_cast<IOI::ZCurve*>(data)->SerializeToJSON(writer);
			}
			else if (typeName[0] == 'E' || typeName[0] == 'e' || typeName.find(".E") != std::string::npos || typeName.find(".e") != std::string::npos)
			{
				std::map<std::string, int> items = enumRegistry.GetEnumByTypeName(typeName);
				int value = *static_cast<int*>(data);

				for (auto it = items.begin(); it != items.end(); it++)
				{
					if (it->second == value)
					{
						writer.String(it->first.c_str());

						break;
					}
				}
			}
			else
			{
				return false;
			}

			return true;
		}
	}
}
