#include "pch.h"
#include <iostream>
#include "ZModel.hpp"
#include "Model/ZRenderPrimitive.hpp"

namespace HM5ResourceTool
{
	bool ZModel::ConvertRenderPrimitiveToOBJ(const std::string& primFilePath, const std::string& objFilePath)
	{
		try
		{
			ZRenderPrimitive renderPrimitive;

			renderPrimitive.Deserialize(primFilePath);
			renderPrimitive.ConvertToOBJ(objFilePath);
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

	bool ZModel::ConvertRenderPrimitiveToOBJ(const void* data, const unsigned int dataSize, const std::string& objFilePath)
	{
		try
		{
			ZRenderPrimitive renderPrimitive;

			renderPrimitive.Deserialize(data, dataSize);
			renderPrimitive.ConvertToOBJ(objFilePath);
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

	bool ZModel::ConvertRenderPrimitiveToGLTF(const std::string& primFilePath, const std::string& gltfFilePath)
	{
		try
		{
			ZRenderPrimitive renderPrimitive;

			renderPrimitive.Deserialize(primFilePath);
			renderPrimitive.ConvertToGLTF(gltfFilePath);
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

	bool ZModel::ConvertRenderPrimitiveToGLTF(const void* data, const unsigned int dataSize, const std::string& gltfFilePath)
	{
		try
		{
			ZRenderPrimitive renderPrimitive;

			renderPrimitive.Deserialize(data, dataSize);
			renderPrimitive.ConvertToGLTF(gltfFilePath);
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
