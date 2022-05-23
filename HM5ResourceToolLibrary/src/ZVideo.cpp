#include "pch.h"
#include <format>
#include <iostream>
#include "ZVideo.hpp"

namespace HM5ResourceTool
{
	bool ZVideo::ConvertPCBinkVidToBINK(const std::string& pcBinkVidPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(pcBinkVidPath);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition()) - 24;

			binaryReader.GetInputStream()->Seek(24, IO::IInputStream::ESeekOrigin::Begin);

			IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(outputFilePath);

			binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetCurrentPointer(), size);
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

	bool ZVideo::ConvertPCBinkVidToBINK(const std::string& pcBinkVidPath, void** binkData, unsigned int& binkSize)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(pcBinkVidPath);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition()) - 24;

			binaryReader.GetInputStream()->Seek(24, IO::IInputStream::ESeekOrigin::Begin);

			binkSize = static_cast<unsigned int>(binaryReader.GetInputStream()->GetSize());
			*binkData = operator new(binkSize);

			memcpy(*binkData, binaryReader.GetInputStream()->GetCurrentPointer(), binkSize);
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

	bool ZVideo::ConvertBINKToPCBinkVid(const std::string& binkPath)
	{
		try
		{
			std::string pcBinkVidPath = std::format("{}.pc_binkvid", binkPath.substr(0, binkPath.find_last_of('.')));
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(binkPath);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition());

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::Begin);

			IO::ZBinaryWriter binaryWriter(pcBinkVidPath);

			binaryWriter.WriteUInt32(0x45444956); //EDIV
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(size);
			binaryWriter.WriteUInt32(0xFFFFFFFF);
			binaryWriter.WriteUInt32(0xFFFFFFFF);

			for (int i = 0; i < 16; i++)
			{
				binaryWriter.WriteByte(0);
			}

			binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetBuffer(), size);
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
