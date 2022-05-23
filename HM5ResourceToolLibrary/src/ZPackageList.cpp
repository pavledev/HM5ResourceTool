#include "pch.h"
#include "..\include\ZPackageList.hpp"

namespace HM5ResourceTool
{
	bool ZPackageList::ConvertPackgeListToJSON(const std::string& packageListPath, std::string& jsonOutput)
	{
		try
		{
			HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(packageListPath);
			IOI::ZBinaryDeserializer binaryDeserializer;

			binaryReader.GetInputStream()->Seek(24, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

			unsigned int dataSize = binaryReader.GetInputStream()->GetSize() - 24;
			void* data = binaryReader.GetInputStream()->ReadBytes(dataSize);

			IOI::SPackageListData* packageListData = static_cast<IOI::SPackageListData*>(binaryDeserializer.Deserialize(data, dataSize));
			jsonOutput = packageListData->SerializeToJSON();

			operator delete(packageListData, std::align_val_t(binaryDeserializer.GetAlignment()));
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
