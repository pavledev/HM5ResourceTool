#include "pch.h"
#include "ZResourceLibrary.hpp"

namespace HM5ResourceToolCLI
{
	ZResourceLibrary::ZResourceLibrary(HM5ResourceTool::ZResourceLibrary* resourceLibrary)
	{
		instance = resourceLibrary;
	}

	void ZResourceLibrary::LoadResourceData(const unsigned int resourceIndex)
	{
		instance->LoadResourceData(resourceIndex);
	}

	String^ ZResourceLibrary::GetResourceID()
	{
		return marshal_as<String^>(instance->GetResourceID());
	}

	String^ ZResourceLibrary::GetFilePath()
	{
		return marshal_as<String^>(instance->GetFilePath());
	}

	ZRuntimeResourceID^ ZResourceLibrary::GetRuntimeResourceID()
	{
		return gcnew ZRuntimeResourceID(instance->GetRuntimeResourceID());
	}

	SResourceHeaderHeader^ ZResourceLibrary::GetResourceHeaderHeader()
	{
		return gcnew SResourceHeaderHeader(instance->GetResourceHeaderHeader());
	}

	List<ZResource^>^ ZResourceLibrary::GetResources()
	{
		std::vector<HM5ResourceTool::ZResource*>* resources = instance->GetResources();
		unsigned int size = resources->size();
		List<ZResource^>^ result = gcnew List<ZResource^>(size);

		for (unsigned int i = 0; i < size; i++)
		{
			ZResource^ resource = gcnew ZResource(resources->operator[](i));

			result->Add(resource);
		}

		return result;
	}

	/*void ZResourceLibrary::SetResourceID(String^ resourceID)
	{
		instance->SetResourceID()
	}

	void ZResourceLibrary::SetFilePath(String^ filePath)
	{
		this->resourceID = resourceID;
	}*/
}
