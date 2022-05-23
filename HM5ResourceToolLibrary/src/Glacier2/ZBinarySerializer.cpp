#include "pch.h"
#include "Glacier2/ZBinarySerializer.hpp"
#include "Glacier2/TArray.hpp"
#include "Glacier2/ZVariant.hpp"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/Math/SMatrix43.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/ZCurve.hpp"

namespace IOI
{
	ZBinarySerializer::ZBinarySerializer()
	{
		this->buffer = nullptr;
		this->layoutPointer = 0;
		this->capacity = 0;
		this->maxAlignment = 0;
		this->currentEndian = Endian::little;
	}

	ZBinarySerializer::~ZBinarySerializer()
	{
		//free(buffer);
	}

	unsigned int ZBinarySerializer::ReserveLayoutFor(const unsigned int count, const unsigned int typeSize, const unsigned int typeAlignment, const unsigned int headerSize, const bool useMaxAlignment)
	{
		if (typeAlignment > maxAlignment)
		{
			maxAlignment = typeAlignment;
		}

		unsigned int typeAlignment2;

		if (useMaxAlignment)
		{
			typeAlignment2 = maxAlignment;
		}
		else
		{
			typeAlignment2 = typeAlignment;
		}

		unsigned int result = ~(typeAlignment2 - 1) & (typeAlignment2 + layoutPointer - 1);

		if (result - layoutPointer < headerSize)
		{
			result = ~(typeAlignment2 - 1) & (layoutPointer + typeAlignment2 + headerSize - 1);
		}

		layoutPointer = result;

		if (count * typeSize == 0)
		{
			++layoutPointer;
		}
		else
		{
			layoutPointer += count * typeSize;
		}

		return result;
	}

	unsigned int ZBinarySerializer::GetLayoutPointer()
	{
		return layoutPointer;
	}

	unsigned char ZBinarySerializer::GetMaxAlignment()
	{
		return maxAlignment;
	}

	void ZBinarySerializer::SetLayoutPointer(unsigned int layoutPointer)
	{
		this->layoutPointer = layoutPointer;
	}

	void ZBinarySerializer::RecordOffsetForRebasing(const unsigned int offset)
	{
		rebaseLocations.insert(offset);
	}

	void ZBinarySerializer::RecordOffsetForTypeIDReindexing(const unsigned int offset)
	{
		typeReindexLocations.push_back(offset);
	}

	void ZBinarySerializer::RecordRuntimeResourceIDOffset(const unsigned int offset)
	{
		runtimeResourceIDOffsets.push_back(offset);
	}

	unsigned int ZBinarySerializer::TypeIdToIndex(STypeID* typeID)
	{
		unsigned int typeIndex = typeIDs.size();

		for (unsigned int i = 0; i < typeIDs.size(); i++)
		{
			if (typeIDs[i] == typeID)
			{
				typeIndex = i;

				break;
			}
		}

		if (typeIndex == typeIDs.size())
		{
			typeIDs.push_back(typeID);
		}

		return typeIndex;
	}

	void ZBinarySerializer::WriteToFileStream(const std::string& filePath, std::unordered_map<unsigned long long, unsigned int>* references)
	{
		void* data = nullptr;
		unsigned int dataSize = 0;

		WriteToMemoryStream(&data, dataSize, references);

		HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(filePath);

		binaryWriter.WriteBytes(data, dataSize);
	}

	void ZBinarySerializer::WriteToMemoryStream(void** data, unsigned int& dataSize, std::unordered_map<unsigned long long, unsigned int>* references)
	{
		std::vector<ZBinarySerializer::Section> sections = GenerateSections();
		int sizeOfAllSections = 0;

		for (unsigned int i = 0; i < sections.size(); i++)
		{
			sizeOfAllSections += sections[i].size;
		}

		sizeOfAllSections += sections.size() * 4;

		uintptr_t capacity = 16 + layoutPointer + sizeOfAllSections;

		*data = calloc(1, capacity);

		HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(*data, capacity);

		binaryWriter.WriteUInt32(0x324E4942);
		binaryWriter.WriteByte(currentEndian == Endian::big);
		binaryWriter.WriteByte(maxAlignment);
		binaryWriter.WriteByte(static_cast<unsigned char>(sections.size()));
		binaryWriter.WriteByte(0);

		const unsigned char dataLength0 = (layoutPointer & 0xFF000000u) >> 24;
		const unsigned char dataLength1 = (layoutPointer & 0xFF0000) >> 16;
		const unsigned char dataLength2 = (layoutPointer & 0xFF00) >> 8;
		const unsigned char dataLength3 = layoutPointer & 0xFFu;

		binaryWriter.WriteByte(dataLength0);
		binaryWriter.WriteByte(dataLength1);
		binaryWriter.WriteByte(dataLength2);
		binaryWriter.WriteByte(dataLength3);

		binaryWriter.WriteUInt32(0);

		unsigned int previousRebaseLocation = 0;
		HM5ResourceTool::IO::ZBinaryWriter rebaseSectionBinaryWriter = HM5ResourceTool::IO::ZBinaryWriter(sections[0].data, sections[0].size);

		rebaseSectionBinaryWriter.GetOutputStream()->Seek(4, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);

		for (auto it = rebaseLocations.begin(); it != rebaseLocations.end(); ++it)
		{
			const unsigned int rebaseLocation = *it;
			unsigned int value = *reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(buffer) + rebaseLocation);

			if (value == 0)
			{
				--value;
			}

			value -= rebaseLocation;

			WriteToMemory(&value, sizeof(unsigned int), rebaseLocation);

			rebaseSectionBinaryWriter.WriteUInt32(rebaseLocation - previousRebaseLocation);

			previousRebaseLocation = rebaseLocation;
		}

		if (!typeIDs.empty())
		{
			unsigned int previousTypeReindexLocation = 0;
			HM5ResourceTool::IO::ZBinaryWriter typeReindexingSectionBinaryWriter = HM5ResourceTool::IO::ZBinaryWriter(sections[1].data, sections[1].size);

			typeReindexingSectionBinaryWriter.GetOutputStream()->Seek(4, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);

			for (unsigned int i = 0; i < typeReindexLocations.size(); i++)
			{
				unsigned int typeReindexLocation = typeReindexLocations[i];

				typeReindexingSectionBinaryWriter.WriteUInt32(typeReindexLocation - previousTypeReindexLocation);

				previousTypeReindexLocation = typeReindexLocation;
			}
		}

		if (!runtimeResourceIDOffsets.empty())
		{
			HM5ResourceTool::IO::ZBinaryWriter runimeResourceIDReindexingSectionBinaryWriter = HM5ResourceTool::IO::ZBinaryWriter(sections[2].data, sections[2].size);

			runimeResourceIDReindexingSectionBinaryWriter.GetOutputStream()->Seek(4, HM5ResourceTool::IO::IOutputStream::ESeekOrigin::Begin);

			for (unsigned int i = 0; i < runtimeResourceIDOffsets.size(); i++)
			{
				const unsigned int offset = runtimeResourceIDOffsets[i];
				const ZRuntimeResourceID runtimeResourceID = *reinterpret_cast<ZRuntimeResourceID*>(reinterpret_cast<uintptr_t>(buffer) + offset);
				unsigned int index = references->operator[](runtimeResourceID.GetID());

				if (runtimeResourceID.GetID() == 0xFFFFFFFFFFFFFFFF)
				{
					*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(buffer) + offset) = 0xFFFFFFFF;
					*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(buffer) + offset + 4) = 0xFFFFFFFF;
				}
				else
				{
					*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(buffer) + offset) = 0;
					*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(buffer) + offset + 4) = index;
				}

				runimeResourceIDReindexingSectionBinaryWriter.WriteUInt32(runtimeResourceIDOffsets[i]);
			}
		}

		binaryWriter.WriteBytes(buffer, layoutPointer);

		for (unsigned int i = 0; i < sections.size(); i++)
		{
			binaryWriter.WriteUInt32(sections[i].type);
			binaryWriter.WriteUInt32(sections[i].size);
			binaryWriter.WriteBytes(sections[i].data, sections[i].size);
		}

		*data = binaryWriter.GetOutputStream()->GetBuffer();
		dataSize = binaryWriter.GetOutputStream()->GetPosition();
	}

	void ZBinarySerializer::EnsureCapacity(const unsigned int size)
	{
		if (size > capacity)
		{
			unsigned int newCapacity = capacity * 2;

			while (newCapacity < size)
			{
				newCapacity *= 2;
			}

			buffer = realloc(buffer, newCapacity);

			unsigned int size2 = newCapacity - capacity;
			void* buffer2 = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + capacity);

			memset(buffer2, 0, size2);

			capacity = newCapacity;
		}
	}

	void ZBinarySerializer::WriteToMemory(const void* data, const unsigned int bytes, const unsigned int offset)
	{
		if (offset == 1344)
		{
			std::string s = "s";
		}

		EnsureCapacity(bytes + offset);

		void* buffer2 = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + offset);

		memcpy(buffer2, data, bytes);
	}

	unsigned int ZBinarySerializer::GetAlignedOffset(const unsigned int typeSize, const unsigned int typeAlignment)
	{
		if (typeAlignment > maxAlignment)
		{
			maxAlignment = typeAlignment;
		}

		unsigned int result = ~(typeAlignment - 1) & (layoutPointer + typeAlignment - 1);

		layoutPointer = result;

		if (!typeSize)
		{
			++layoutPointer;
		}
		else
		{
			layoutPointer += typeSize;
		}

		return result;
	}

	unsigned int ZBinarySerializer::GetAlignedLayoutPointer(const unsigned int layoutPointer, const unsigned int alignment)
	{
		return ~(alignment - 1) & (layoutPointer + alignment - 1);
	}

	std::optional<unsigned int> ZBinarySerializer::GetDataOffset(const ZVariant* variant)
	{
		auto it = typeIDs2.find(variant->GetTypeID());

		if (it == typeIDs2.end())
		{
			return std::nullopt;
		}

		auto& dataPointers = it->second;

		for (auto it2 = dataPointers.begin(); it2 != dataPointers.end(); ++it2)
		{
			if (variant->GetData() == it2->first)
			{
				return std::make_optional(it2->second);
			}
		}

		return std::nullopt;
	}

	void ZBinarySerializer::SetDataOffset(const ZVariant* variant, unsigned int offset)
	{
		auto it = typeIDs2.find(variant->GetTypeID());

		if (it != typeIDs2.end())
		{
			it->second[variant->GetData()] = offset;

			return;
		}

		std::unordered_map<void*, unsigned int> dataPointers;

		dataPointers[variant->GetData()] = offset;
		typeIDs2[variant->GetTypeID()] = dataPointers;
	}

	ZBinarySerializer::Section ZBinarySerializer::GenerateRebaseLocationsSection()
	{
		unsigned int capacity = (rebaseLocations.size() + 1) * sizeof(unsigned int);
		void* data = calloc(1, capacity);
		HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(data, capacity);

		binaryWriter.WriteUInt32(rebaseLocations.size());

		for (auto it = rebaseLocations.begin(); it != rebaseLocations.end(); ++it)
		{
			binaryWriter.WriteUInt32(*it);
		}

		return Section(0x12EBA5ED, binaryWriter.GetOutputStream()->GetPosition(), binaryWriter.GetOutputStream()->GetBuffer());
	}

	ZBinarySerializer::Section ZBinarySerializer::GenerateTypeReindexingSection()
	{
		unsigned int capacity = (typeReindexLocations.size() + typeIDs.size() + 2) * sizeof(unsigned int);
		void* data = calloc(1, capacity);
		HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(data, capacity);

		binaryWriter.WriteUInt32(typeReindexLocations.size());

		for (unsigned int i = 0; i < typeReindexLocations.size(); i++)
		{
			binaryWriter.WriteUInt32(typeReindexLocations[i]);
		}

		binaryWriter.WriteUInt32(typeIDs.size());

		for (unsigned int i = 0; i < typeIDs.size(); i++)
		{
			binaryWriter.AlignTo(4);

			binaryWriter.WriteUInt32(i);
			binaryWriter.WriteInt32(-1);
			binaryWriter.WriteUInt32(strlen(typeIDs[i]->pTypeInfo->pszTypeName) + 1);
			binaryWriter.WriteString(typeIDs[i]->pTypeInfo->pszTypeName);
		}

		return Section(0x3989BF9F, binaryWriter.GetOutputStream()->GetPosition(), binaryWriter.GetOutputStream()->GetBuffer());
	}

	ZBinarySerializer::Section ZBinarySerializer::GenerateRunimeResourceIDReindexingSection()
	{
		unsigned int capacity = (runtimeResourceIDOffsets.size() + 1) * sizeof(unsigned int);
		void* data = calloc(1, capacity);
		HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(data, capacity);

		binaryWriter.WriteUInt32(runtimeResourceIDOffsets.size());

		for (unsigned int i = 0; i < runtimeResourceIDOffsets.size(); i++)
		{
			binaryWriter.WriteUInt32(runtimeResourceIDOffsets[i]);
		}

		return Section(0x578FBCEE, binaryWriter.GetOutputStream()->GetPosition(), binaryWriter.GetOutputStream()->GetBuffer());
	}

	std::vector<ZBinarySerializer::Section> ZBinarySerializer::GenerateSections()
	{
		std::vector<Section> sections;

		if (!rebaseLocations.empty())
		{
			sections.push_back(GenerateRebaseLocationsSection());
		}

		if (!typeIDs.empty())
		{
			sections.push_back(GenerateTypeReindexingSection());
		}

		if (!runtimeResourceIDOffsets.empty())
		{
			sections.push_back(GenerateRunimeResourceIDReindexingSection());
		}

		return sections;
	}

	ZBinarySerializer::Section::Section(unsigned int type, unsigned int size, void* data)
	{
		this->type = type;
		this->size = size;
		this->data = data;
	}
}
