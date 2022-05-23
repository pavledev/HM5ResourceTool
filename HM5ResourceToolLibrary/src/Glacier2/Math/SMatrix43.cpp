#include "pch.h"
#include "Glacier2/Math/SMatrix43.hpp"

namespace IOI
{
    void SMatrix43::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("Rot");
        Rot.ConvertRotationMatrixToEulerAngles().SerializeToJSON(writer);

        writer.String("Trans");
        Trans.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SMatrix43::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int rotOffset = offset + offsetof(SMatrix43, Rot);
        unsigned int transOffset = offset + offsetof(SMatrix43, Trans);

        Rot.SerializeToMemory(binarySerializer, rotOffset);
        Trans.SerializeToMemory(binarySerializer, transOffset);
    }

    SMatrix43* SMatrix43::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SMatrix43* matrix = new SMatrix43();

        matrix->Rot = SMatrix33::ConvertEulerAnglesToRotationMatrix(*SVector3::DeserializeFromJSON(object["Rot"].GetObj()));
        matrix->Trans = *SVector3::DeserializeFromJSON(object["Trans"].GetObj());

        return matrix;
    }

    bool SMatrix43::operator==(const SMatrix43& other)
    {
        return Rot == other.Rot && Trans == other.Trans;
    }
}
