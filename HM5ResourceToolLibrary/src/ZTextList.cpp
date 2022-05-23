#include "pch.h"
#include <iostream>
#include "ZTextList.hpp"
#include "IO/ZBinaryReader.hpp"

namespace HM5ResourceTool
{
	bool ZTextList::ConvertTextListToText(const std::string& textListPath, std::string& output)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(textListPath);

			unsigned int i = 0;
			unsigned int numberOfEntries = binaryReader.ReadUInt32();

			while (i < numberOfEntries)
			{
				int key = binaryReader.ReadUInt32();
				int entryLength = binaryReader.ReadUInt32();
				std::string entry = binaryReader.ReadString(entryLength);

				output += entry + "\n";

				i++;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTextList::ConvertTextListToText(const void* data, const unsigned int dataSize, std::string& output)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

			unsigned int i = 0;
			unsigned int numberOfEntries = binaryReader.ReadUInt32();

			while (i < numberOfEntries)
			{
				int key = binaryReader.ReadUInt32();
				int entryLength = binaryReader.ReadUInt32();
				std::string entry = binaryReader.ReadString(entryLength);

				output += entry + "\n";

				i++;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}
}
