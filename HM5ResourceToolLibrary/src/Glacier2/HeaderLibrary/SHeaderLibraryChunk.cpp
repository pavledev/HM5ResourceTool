#include "pch.h"
#include "Glacier2/HeaderLibrary/SHeaderLibraryChunk.hpp"

namespace IOI
{
    void SHeaderLibraryChunk::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("sLibraryID");
        sLibraryID.SerializeToJSON(writer);

        writer.String("nOffset");
        writer.Uint(nOffset);

        writer.String("nFlags");
        writer.Int(nFlags);

        writer.String("localizationCategory");
        writer.Uint(localizationCategory);

        writer.String("sLanguages");
        sLanguages.SerializeToJSON(writer);

        writer.String("states");
        states.SerializeToJSON(writer);

        writer.String("resourceHeaders");

        TArray<ZString> resourceHeaders = TArray<ZString>(this->resourceHeaders.Size());

        for (unsigned int i = 0; i < this->resourceHeaders.Size(); i++)
        {
            char* resourceHeader = new char(5);

            memcpy_s(resourceHeader, 4, this->resourceHeaders[i].GetStart(), 4);

            resourceHeader[4] = '\0';

            std::reverse(resourceHeader, resourceHeader + strlen(resourceHeader));

            resourceHeaders.PushBack(ZString(resourceHeader));
        }

        resourceHeaders.SerializeToJSON(writer);

        writer.String("ridMappingIndices");
        ridMappingIndices.SerializeToJSON(writer);

        writer.String("ridMappingIDs");
        ridMappingIDs.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SHeaderLibraryChunk::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int libraryIDOffset = offset + offsetof(SHeaderLibraryChunk, sLibraryID);
        unsigned int offsetOffset = offset + offsetof(SHeaderLibraryChunk, nOffset);
        unsigned int flagsOffset = offset + offsetof(SHeaderLibraryChunk, nFlags);
        unsigned int localizationCategoryOffset = offset + offsetof(SHeaderLibraryChunk, localizationCategory);
        unsigned int languagesOffset = offset + offsetof(SHeaderLibraryChunk, sLanguages);
        unsigned int statesOffset = offset + offsetof(SHeaderLibraryChunk, states);
        unsigned int resourceHeadersOffset = offset + offsetof(SHeaderLibraryChunk, resourceHeaders);
        unsigned int ridMappingIndicesOffset = offset + offsetof(SHeaderLibraryChunk, ridMappingIndices);
        unsigned int ridMappingIDsOffset = offset + offsetof(SHeaderLibraryChunk, ridMappingIDs);

        binarySerializer.WriteToMemory(&nOffset, sizeof(int), offsetOffset);
        binarySerializer.WriteToMemory(&nFlags, sizeof(int), flagsOffset);
        binarySerializer.WriteToMemory(&localizationCategory, sizeof(int), localizationCategoryOffset);

        sLibraryID.SerializeToMemory(binarySerializer, libraryIDOffset);
        sLanguages.SerializeToMemory(binarySerializer, languagesOffset);
        states.SerializeToMemory(binarySerializer, statesOffset);
        resourceHeaders.SerializeToMemory(binarySerializer, resourceHeadersOffset);
        ridMappingIndices.SerializeToMemory(binarySerializer, ridMappingIndicesOffset);
        ridMappingIDs.SerializeToMemory(binarySerializer, ridMappingIDsOffset);
    }
}
