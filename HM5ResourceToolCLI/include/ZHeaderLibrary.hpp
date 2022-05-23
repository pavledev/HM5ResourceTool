#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZHeaderLibrary.hpp"
#include "ZResource.hpp"
#include "ZResourceLibrary.hpp"

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
	public ref class ZHeaderLibrary : public ManagedObject<HM5ResourceTool::ZHeaderLibrary>
	{
	public:
		ZHeaderLibrary(String^ headerLibraryPath);
		bool ParseHeaderLibrary(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, bool readData);
		bool ExtractResourceLibrary(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, String^ exportFolderPath, String^ resourceID);
		bool ExtractResourceLibraries(String^ gamePath, ZHeaderLibrary^ globalHeaderLibrary, String^ exportFolderPath);
		void LoadResourceData(const unsigned int libraryIndex, const unsigned int resourceIndex);
		void LoadResourceReferences(const unsigned int libraryIndex, const unsigned int resourceIndex, ZHeaderLibrary^ globalHeaderLibrary);
		List<ZResource^>^ GetReferences();
		List<ZResourceLibrary^>^ GetResourceLibraries();
		void SerializeToJSON([Out] String^% jsonOutput);
		void ExtractPropertyNames(bool closeFile, bool exit);
		void ExtractTypeNames(bool closeFile, bool exit);
	};
}
