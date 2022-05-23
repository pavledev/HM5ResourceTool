#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "Glacier2/HeaderLibrary/SHeaderLibrary.hpp"
#include "ZResourceLibrary.hpp"

namespace HM5ResourceTool
{
	class ZHeaderLibrary
	{
	public:
		__declspec(dllexport) ZHeaderLibrary(const std::string& headerLibraryPath);
		__declspec(dllexport) ~ZHeaderLibrary();
		__declspec(dllexport) bool ParseHeaderLibrary(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary = nullptr, bool readData = true);
		__declspec(dllexport) bool ExtractResourceLibrary(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary, const std::string& exportFolderPath, const std::string& resourceID);
		__declspec(dllexport) bool ExtractResourceLibraries(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary, const std::string& exportFolderPath);
		void ParseReferencesChunk();
		void ParseReferencesChunk(IO::ZBinaryReader& headerBinaryReader, ZResource& resource, ZHeaderLibrary* globalHeaderLibrary);
		__declspec(dllexport) void LoadResourceData(const unsigned int libraryIndex, const unsigned int resourceIndex);
		__declspec(dllexport) void LoadResourceReferences(const unsigned int libraryIndex, const unsigned int resourceIndex, ZHeaderLibrary* globalHeaderLibrary);
		__declspec(dllexport) std::vector<ZResource>* GetReferences();
		const std::vector<ZResource>* GetReferences() const;
		__declspec(dllexport) std::vector<ZResourceLibrary*>* GetResourceLibraries();
		void DeserializeHeaderLibrary();
		__declspec(dllexport) void SerializeToJSON(std::string& jsonOutput);
		int GetIndexOfResourceLibrary(const IOI::ZRuntimeResourceID& runtimeResourceID, bool isGlobalResourceLibrary = false) const;
		unsigned int GetIndexOfResource(const IOI::ZRuntimeResourceID& runtimeResourceID) const;
		__declspec(dllexport) void ExtractPropertyNames(bool closeFile, bool exit);
		__declspec(dllexport) void ExtractTypeNames(bool closeFile, bool exit);

	private:
		std::string headerLibraryPath;
		IO::ZBinaryReader* headerBinaryReader;
		IOI::SResourceHeaderHeader* resourceHeaderHeader;
		IOI::SHeaderLibrary* headerLibrary;
		std::vector<ZResource>* references;
		std::vector<ZResourceLibrary*>* resourceLibraries;
		unsigned char alignment;
	};
}
