#include "pch.h"
#include "ZHeaderLibrary.hpp"

namespace HM5ResourceToolCLI
{
	ZHeaderLibrary::ZHeaderLibrary(String^ headerLibraryPath) : ManagedObject(new HM5ResourceTool::ZHeaderLibrary(marshal_as<std::string>(headerLibraryPath)))
	{

	}

	bool ZHeaderLibrary::ParseHeaderLibrary(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, bool readData)
	{
		std::string gamePath2 = marshal_as<std::string>(gamePath);

		if (globalHeaderLibrary)
		{
			return instance->ParseHeaderLibrary(gamePath2, globalHeaderLibrary->GetInstance(), readData);
		}

		return instance->ParseHeaderLibrary(gamePath2, nullptr, readData);
	}

	bool ZHeaderLibrary::ExtractResourceLibrary(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, String^ exportFolderPath, String^ resourceID)
	{
		std::string gamePath2 = marshal_as<std::string>(gamePath);
		std::string exportFolderPath2 = marshal_as<std::string>(exportFolderPath);
		std::string resourceID2 = marshal_as<std::string>(resourceID);

		return instance->ExtractResourceLibrary(gamePath2, globalHeaderLibrary->GetInstance(), exportFolderPath2, resourceID2);
	}

	bool ZHeaderLibrary::ExtractResourceLibraries(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, String^ exportFolderPath)
	{
		std::string gamePath2 = marshal_as<std::string>(gamePath);
		std::string exportFolderPath2 = marshal_as<std::string>(exportFolderPath);

		return instance->ExtractResourceLibraries(gamePath2, globalHeaderLibrary->GetInstance(), exportFolderPath2);
	}

	void ZHeaderLibrary::LoadResourceData(const unsigned int libraryIndex, const unsigned int resourceIndex)
	{
		instance->LoadResourceData(libraryIndex, resourceIndex);
	}

	void ZHeaderLibrary::LoadResourceReferences(const unsigned int libraryIndex, const unsigned int resourceIndex, ZHeaderLibrary^ globalHeaderLibrary)
	{
		instance->LoadResourceReferences(libraryIndex, resourceIndex, globalHeaderLibrary->GetInstance());
	}

	List<ZResource^>^ ZHeaderLibrary::GetReferences()
	{
		std::vector<HM5ResourceTool::ZResource>* references = instance->GetReferences();
		unsigned int size = references->size();
		List<ZResource^>^ result = gcnew List<ZResource^>(size);

		for (unsigned int i = 0; i < size; i++)
		{
			ZResource^ resource = gcnew ZResource(&references->operator[](i));

			result->Add(resource);
		}

		return result;
	}

	List<ZResourceLibrary^>^ ZHeaderLibrary::GetResourceLibraries()
	{
		std::vector<HM5ResourceTool::ZResourceLibrary*>* resourceLibraries = instance->GetResourceLibraries();
		unsigned int size = resourceLibraries->size();
		List<ZResourceLibrary^>^ result = gcnew List<ZResourceLibrary^>(size);

		for (unsigned int i = 0; i < size; i++)
		{
			ZResourceLibrary^ resourceLibrary = gcnew ZResourceLibrary(resourceLibraries->operator[](i));

			result->Add(resourceLibrary);
		}

		return result;
	}

	void ZHeaderLibrary::SerializeToJSON([Out] String^% jsonOutput)
	{
		std::string jsonOutput2;

		instance->SerializeToJSON(jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());
	}

	void ZHeaderLibrary::ExtractPropertyNames(bool closeFile, bool exit)
	{
		instance->ExtractPropertyNames(closeFile, exit);
	}

	void ZHeaderLibrary::ExtractTypeNames(bool closeFile, bool exit)
	{
		instance->ExtractTypeNames(closeFile, exit);
	}
}
