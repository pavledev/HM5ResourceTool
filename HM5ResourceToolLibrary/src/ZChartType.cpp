#include "pch.h"
#include <iostream>
#include <stdexcept>
#include "ZChartType.hpp"
#include "IO/ZBinaryReader.hpp"

namespace HM5ResourceTool
{
	bool ZChartType::ConvertChartTypeToText(const void* data, const unsigned int dataSize, std::string& output)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

			unsigned int i = 0;
			unsigned int entryCount = binaryReader.ReadUInt32();
			unsigned int animationNamesOffset = entryCount * 4 + entryCount * 4 + 4;

			binaryReader.GetInputStream()->Seek(animationNamesOffset, IO::IInputStream::ESeekOrigin::Begin);

			while (i < entryCount)
			{
				std::string entry = binaryReader.ReadString();

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
