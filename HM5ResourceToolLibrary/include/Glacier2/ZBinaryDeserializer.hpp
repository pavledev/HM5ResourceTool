#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "IO/ZJSONDeserializer.hpp"
#include "Registry/ZTypeRegistry.hpp"
#include "ZResource.hpp"

namespace IOI
{
	class ZBinaryDeserializer
	{
	public:
		ZBinaryDeserializer();
		void* Deserialize(const std::string& filePath, const std::vector<HM5ResourceTool::ZResource*>* references = nullptr);
		void* Deserialize(const void* buffer, const size_t size, const std::vector<HM5ResourceTool::ZResource*>* references = nullptr);
		void* Deserialize(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const std::vector<HM5ResourceTool::ZResource*>* references = nullptr);
		const unsigned char GetAlignment() const;

	private:
		enum class Endian
		{
			big = 0,
			little = 1
		};

		ZTypeRegistry typeRegistry;
		unsigned char alignment;
		Endian endian;

		void HandleRebaseSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader, HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter);
		void HandleTypeReindexingSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader, HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter);
		void HandleRuntimeResourceIDReindexingSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader, HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter, const std::vector<HM5ResourceTool::ZResource*>* references = nullptr);
		void Align(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const size_t currentPosition, const size_t alignment);
		STypeID* GetTypeIDFromTypeName(const std::string& typeName);
	};
}
