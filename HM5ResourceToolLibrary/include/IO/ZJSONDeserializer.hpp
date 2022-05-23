#pragma once

#include <string>
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

namespace HM5ResourceTool
{
	namespace IO
	{
		class ZJSONDeserializer
		{
		public:
			ZJSONDeserializer(const std::string& filePath);
			~ZJSONDeserializer();

			template <typename T>
			T* Deserialize()
			{
				rapidjson::IStreamWrapper streamWrapper(stream);
				rapidjson::Document document;

				document.ParseStream(streamWrapper);

				return T::DeserializeFromJSON(document);
			}

			template <typename T>
			void Deserialize(T& type)
			{
				rapidjson::IStreamWrapper streamWrapper(stream);
				rapidjson::Document document;

				document.ParseStream(streamWrapper);

				T::DeserializeFromJSON(document, type);
			}

		private:
			std::ifstream stream;
		};
	}
}
