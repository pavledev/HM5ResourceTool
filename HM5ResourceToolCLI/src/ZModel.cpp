#include "pch.h"
#include "ZModel.hpp"

namespace HM5ResourceToolCLI
{
	ZModel::ZModel() : ManagedObject(new HM5ResourceTool::ZModel())
	{

	}

	bool ZModel::ConvertRenderPrimitiveToOBJ(const array<Byte>^ data, const unsigned int dataSize, String^ objFilePath)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string objFilePath2 = marshal_as<std::string>(objFilePath);

		return HM5ResourceTool::ZModel::ConvertRenderPrimitiveToOBJ(dataPointer, dataSize, objFilePath2);
	}

	bool ZModel::ConvertRenderPrimitiveToGLTF(const array<Byte>^ data, const unsigned int dataSize, String^ objFilePath)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string objFilePath2 = marshal_as<std::string>(objFilePath);

		return HM5ResourceTool::ZModel::ConvertRenderPrimitiveToGLTF(dataPointer, dataSize, objFilePath2);
	}
}
