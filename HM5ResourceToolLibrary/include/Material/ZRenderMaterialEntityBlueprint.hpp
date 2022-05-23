#pragma once

#include <vector>
#include "IO/ZBinaryReader.hpp"

namespace HM5ResourceTool
{
	class ZRenderMaterialEntityBlueprint
	{
	public:
		struct SModifier
		{
			unsigned char type;
			std::string name;
		};

		const std::vector<SModifier*>& GetModifiers();
		void DeserializeEntityData(const std::string& mattFilePath);
		void DeserializeEntityData(const void* data, const unsigned int dataSize);
		void DeserializeEntityData(IO::ZBinaryReader& binaryReader);
		std::string SerializeToJSON();

	private:
		std::vector<SModifier*> modifiers;
	};
}
