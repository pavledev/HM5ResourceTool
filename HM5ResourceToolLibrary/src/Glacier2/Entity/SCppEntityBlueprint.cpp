#include "pch.h"
#include "Glacier2/Entity/SCppEntityBlueprint.hpp"

namespace IOI
{
    std::string SCppEntityBlueprint::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("typeName");
        typeName->SerializeToJSON(writer);

        writer.String("subsets");
        subsets.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    void SCppEntityBlueprint::SerializeToMemory(ZBinarySerializer& binarySerializer)
    {
        unsigned int typeNameOffset = offsetof(SCppEntityBlueprint, typeName);
        unsigned int subsetsOffset = offsetof(SCppEntityBlueprint, subsets);

        typeName->SerializeToMemory(binarySerializer, typeNameOffset);
        subsets.SerializeToMemory(binarySerializer, subsetsOffset);
    }

    SCppEntityBlueprint* SCppEntityBlueprint::DeserializeFromJSON(const rapidjson::Document& document)
    {
        SCppEntityBlueprint* cppEntityBlueprint = new SCppEntityBlueprint();

        cppEntityBlueprint->typeName = STypeID::DeserializeFromJSON(document, "typeName");
        cppEntityBlueprint->subsets = *TArray<SCppEntitySubsetInfo>::DeserializeFromJSON(document["subsets"].GetArray());

        return cppEntityBlueprint;
    }
}
