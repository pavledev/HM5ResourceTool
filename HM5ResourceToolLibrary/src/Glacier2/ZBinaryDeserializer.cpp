#include "pch.h"
#include <Windows.h>
#include <format>
#include "Glacier2/ZBinaryDeserializer.hpp"

namespace IOI
{
	ZBinaryDeserializer::ZBinaryDeserializer()
	{
		char buffer[MAX_PATH];

		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		std::string filePath = std::format("{}/STypeIDs.json", std::string(buffer).substr(0, pos));
		HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

		jsonDeserializer.Deserialize<ZTypeRegistry>(typeRegistry);
	}

	void* ZBinaryDeserializer::Deserialize(const std::string& filePath, const std::vector<HM5ResourceTool::ZResource*>* references)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(filePath);

		return Deserialize(binaryReader, references);
	}

	void* ZBinaryDeserializer::Deserialize(const void* buffer, const size_t size, const std::vector<HM5ResourceTool::ZResource*>* references)
	{
		HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(buffer, size);

		return Deserialize(binaryReader, references);
	}

	void* ZBinaryDeserializer::Deserialize(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const std::vector<HM5ResourceTool::ZResource*>* references)
	{
		char* chars = binaryReader.ReadChars(4);
		std::string magic = std::string(chars, 4);

		delete[] chars;

		if (magic != "BIN2" && magic != "BIN1") //BIN1 is used in PS3 and XBOX360
		{
			throw std::invalid_argument("File format not supported!");
		}

		endian = static_cast<Endian>(binaryReader.ReadByte() == 0);

		binaryReader.SetEndian(static_cast<HM5ResourceTool::IO::IInputStream::Endian>(endian));

		const unsigned char alignment = binaryReader.ReadByte();
		const unsigned char sectionsCount = binaryReader.ReadByte();

		const unsigned char unusedByte = binaryReader.ReadByte();

		const unsigned char dataLength0 = binaryReader.ReadByte();
		const unsigned char dataLength1 = binaryReader.ReadByte();
		const unsigned char dataLength2 = binaryReader.ReadByte();
		const unsigned char dataLength3 = binaryReader.ReadByte();

		const unsigned int dataLength = (dataLength0 << 24) + (dataLength1 << 16) + (dataLength2 << 8) + dataLength3;

		unsigned int unusedDWORD = binaryReader.ReadUInt32();

		void* data = operator new(dataLength, std::align_val_t(alignment));

		binaryReader.ReadBytes(data, dataLength);

		HM5ResourceTool::IO::ZBinaryReader dataSectionBinaryReader(data, dataLength);
		HM5ResourceTool::IO::ZBinaryWriter dataSectionBinaryWriter(data, dataLength);

		dataSectionBinaryReader.SetEndian(static_cast<HM5ResourceTool::IO::IInputStream::Endian>(endian));
		dataSectionBinaryWriter.SetEndian(static_cast<HM5ResourceTool::IO::IOutputStream::Endian>(endian));

		for (unsigned char i = 0; i < sectionsCount; ++i)
		{
			const unsigned int sectionType = binaryReader.ReadUInt32();
			const unsigned int sectionSize = binaryReader.ReadUInt32();

			switch (sectionType)
			{
			case 0x12EBA5ED:
				HandleRebaseSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter);
				break;
			case 0x3989BF9F:
				HandleTypeReindexingSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter);
				break;
			case 0x578FBCEE:
				HandleRuntimeResourceIDReindexingSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter, references);
				break;
			default:
			{
				std::stringstream stream;

				stream << std::hex << sectionType;

				throw std::invalid_argument(std::format("Unknown section type: 0x{}!", stream.str()));
			}
			}
		}

		this->alignment = alignment;

		return data;
	}

	const unsigned char ZBinaryDeserializer::GetAlignment() const
	{
		return alignment;
	}

	void ZBinaryDeserializer::HandleRebaseSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader, HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter)
	{
		const unsigned int numberOfRebaseLocations = binaryReader.ReadUInt32();
		unsigned int rebaseLocationOffset = 0;

		for (unsigned int i = 0; i < numberOfRebaseLocations; ++i)
		{
			if (endian == Endian::little)
			{
				rebaseLocationOffset += binaryReader.ReadUInt32();
			}
			else
			{
				rebaseLocationOffset = binaryReader.ReadUInt32();
			}

			dataSectionbinaryReader.GetInputStream()->Seek(rebaseLocationOffset, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);
			dataSectionBinaryWriter.GetOutputStream()->Seek(rebaseLocationOffset, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);

			//int value = dataSectionbinaryReader.ReadInt32() + rebaseLocationOffset;
			int value = 0;

			if (endian == Endian::little)
			{
				value = dataSectionbinaryReader.ReadInt32() + rebaseLocationOffset;
			}
			else
			{
				value = dataSectionbinaryReader.ReadInt32();
			}

			if (value != -1)
			{
				dataSectionBinaryWriter.WriteUInt32(reinterpret_cast<uintptr_t>(dataSectionbinaryReader.GetInputStream()->GetBuffer()) + value);
			}
			else
			{
				dataSectionBinaryWriter.WriteUInt32(0);
			}
		}
	}

	void ZBinaryDeserializer::HandleTypeReindexingSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader, HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter)
	{
		unsigned int sectionStartPosition = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition());
		unsigned int numberOfOffsetsToReindex = binaryReader.ReadUInt32();
		std::unordered_map<unsigned int, unsigned int> typeIDsToReindex;
		unsigned int typeIDOffset = 0;

		for (unsigned int i = 0; i < numberOfOffsetsToReindex; ++i)
		{
			typeIDOffset += binaryReader.ReadUInt32();

			dataSectionbinaryReader.GetInputStream()->Seek(typeIDOffset, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

			const unsigned int typeIDIndex = dataSectionbinaryReader.ReadUInt32();

			typeIDsToReindex.insert(std::make_pair(typeIDOffset, typeIDIndex));
		}

		const unsigned int numberOfTypeNames = binaryReader.ReadUInt32();
		std::vector<STypeID*> typeIDs(numberOfTypeNames);

		for (unsigned int i = 0; i < numberOfTypeNames; ++i)
		{
			unsigned int currentPosition = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition()) - sectionStartPosition;

			Align(binaryReader, currentPosition, 4);

			const unsigned int typeID = binaryReader.ReadUInt32();
			const int typesize = binaryReader.ReadInt32();
			const unsigned int typeNameLength = binaryReader.ReadUInt32();
			std::string typeName = binaryReader.ReadString(typeNameLength - 1);

			STypeID* type = GetTypeIDFromTypeName(typeName);

			if (!type)
			{
				/*type = new IOI::STypeID();
				IOI::IType* type2 = new IOI::IType(0, 0, 0, typeName.c_str());

				type->pTypeInfo = type2;*/

				throw std::invalid_argument(std::format("Type info for {} isn't available!", typeName));
			}

			typeIDs[typeID] = type;
		}

		for (auto it = typeIDsToReindex.begin(); it != typeIDsToReindex.end(); it++)
		{
			const unsigned int typeIDOffset = it->first;
			const unsigned int typeIDIndex = it->second;

			dataSectionBinaryWriter.GetOutputStream()->Seek(typeIDOffset, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);
			dataSectionBinaryWriter.WriteUInt32(reinterpret_cast<uintptr_t>(typeIDs[typeIDIndex]));
		}
	}

	void ZBinaryDeserializer::HandleRuntimeResourceIDReindexingSection(HM5ResourceTool::IO::ZBinaryReader& binaryReader, HM5ResourceTool::IO::ZBinaryReader& dataSectionbinaryReader,
		HM5ResourceTool::IO::ZBinaryWriter& dataSectionBinaryWriter, const std::vector<HM5ResourceTool::ZResource*>* references)
	{
		if (!references)
		{
			return;
		}

		const unsigned int numberOfOffsetsToReindex = binaryReader.ReadUInt32();

		for (unsigned int i = 0; i < numberOfOffsetsToReindex; ++i)
		{
			const unsigned int runtimeResourceIDOffset = binaryReader.ReadUInt32();

			dataSectionbinaryReader.GetInputStream()->Seek(runtimeResourceIDOffset, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

			const unsigned int idHigh = dataSectionbinaryReader.ReadUInt32();
			const unsigned int idLow = dataSectionbinaryReader.ReadUInt32(); //Index of header reference

			if (idLow != UINT32_MAX)
			{
				ZRuntimeResourceID* runtimeResourceID = references->operator[](idLow)->GetRuntimeResourceID();

				dataSectionBinaryWriter.GetOutputStream()->Seek(runtimeResourceIDOffset, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);
				dataSectionBinaryWriter.WriteBytes(runtimeResourceID, sizeof(ZRuntimeResourceID));
			}
		}
	}

	void ZBinaryDeserializer::Align(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const size_t currentPosition, const size_t alignment)
	{
		if (alignment > 1)
		{
			size_t misalign = currentPosition % alignment;

			if (misalign != 0)
			{
				binaryReader.GetInputStream()->Seek(alignment - misalign, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);
			}
		}
	}

	STypeID* ZBinaryDeserializer::GetTypeIDFromTypeName(const std::string& typeName)
	{
		return typeRegistry.GetTypeIDFromTypeName(typeName);
	}
}
