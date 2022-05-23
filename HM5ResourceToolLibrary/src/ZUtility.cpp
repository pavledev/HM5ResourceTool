#include "pch.h"
#include <sstream>
#include "ZUtility.hpp"

std::string ZUtility::ConvertTypeToString(unsigned int type)
{
	std::stringstream stream;

	stream << std::hex << type;

	std::string hex(stream.str());

	int length = hex.length();
	std::string result;

	for (int i = 0; i < length; i += 2)
	{
		std::string byte = hex.substr(i, 2);
		char chr = static_cast<char>(static_cast<int>(strtol(byte.c_str(), nullptr, 16)));

		result.push_back(chr);
	}

	return result;
}

unsigned int ZUtility::ConvertStringToType(const std::string& string)
{
	unsigned int type;

	type = static_cast<unsigned int>(string[3]) << 24;
	type += static_cast<unsigned int>(string[2]) << 16;
	type += static_cast<unsigned int>(string[1]) << 8;
	type += static_cast<unsigned int>(string[0]);

	return type;
}

std::string ZUtility::ConvertResourceReferenceFlagsToString(const IOI::EResourceReferenceFlags resourceReferenceFlags)
{
	std::string result;

	switch (resourceReferenceFlags)
	{
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_NO_FLAGS:
		result = "No Flags";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_OFFSET_BITS:
		result = "Offset Bits";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_OFFSET_BITCOUNT:
		result = "Offset Bitcount";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_FLAG_BITS:
		result = "Flag Bits";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_TYPE_OF_STREAMING_ENTITY:
		result = "Type Of Streaming Entity";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_STATE_STREAMED:
		result = "State Streamed";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_MEDIA_STREAMED:
		result = "Media Streamed";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_INSTALL_DEPENDENCY:
		result = "Install Dependency";
		break;
	case IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_CYCLIC:
		result = "Cyclic";
		break;
	default:
		break;
	}

	return result;
}

std::string ZUtility::GetBaseNameFromPath(const std::string& path)
{
	const std::string fileName = path.substr(path.find_last_of("/\\") + 1);
	const size_t count = fileName.find_last_of('.');

	return fileName.substr(0, count);
}
