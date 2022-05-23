#include "pch.h"
#include <math.h>
#include "Glacier2/Math/SMatrix33.hpp"
#include "ZUtility.hpp"

namespace IOI
{
    void SMatrix33::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("XAxis");
        XAxis.SerializeToJSON(writer);

        writer.String("YAxis");
        YAxis.SerializeToJSON(writer);

        writer.String("ZAxis");
        ZAxis.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SMatrix33::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int xAxisOffset = offset + offsetof(SMatrix33, XAxis);
        unsigned int yAxisOffset = offset + offsetof(SMatrix33, YAxis);
        unsigned int zAxisOffset = offset + offsetof(SMatrix33, ZAxis);

        XAxis.SerializeToMemory(binarySerializer, xAxisOffset);
        YAxis.SerializeToMemory(binarySerializer, yAxisOffset);
        ZAxis.SerializeToMemory(binarySerializer, zAxisOffset);
    }

    SMatrix33* SMatrix33::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SMatrix33* matrix = new SMatrix33();

        matrix->XAxis = *SVector3::DeserializeFromJSON(object["XAxis"].GetObj());
        matrix->YAxis = *SVector3::DeserializeFromJSON(object["YAxis"].GetObj());
        matrix->ZAxis = *SVector3::DeserializeFromJSON(object["ZAxis"].GetObj());

        return matrix;
    }

    bool SMatrix33::operator==(const SMatrix33& other)
    {
        return XAxis == other.XAxis && YAxis == other.YAxis && ZAxis == other.ZAxis;
    }

    SVector3 SMatrix33::ConvertRotationMatrixToEulerAngles()
    {
        SVector3 result;

        result.y = std::asin(ZUtility::Clamp(m13, -1.f, 1.f));

        if (std::abs(m13) < 0.99999)
        {
            result.x = std::atan2(-m23, m33);
            result.z = std::atan2(-m12, m11);
        }
        else
        {
            result.x = std::atan2(m32, m22);
            result.z = 0;
        }

        return result;
    }

    SMatrix33 SMatrix33::ConvertEulerAnglesToRotationMatrix(const SVector3& vector)
    {
        SMatrix33 result;

        float x = vector.x, y = vector.y, z = vector.z;
        float a = std::cos(x), b = std::sin(x);
        float c = std::cos(y), d = std::sin(y);
        float e = std::cos(z), f = std::sin(z);

        float ae = a * e, af = a * f, be = b * e, bf = b * f;

        result.m11 = c * e;
        result.m12 = -c * f;
        result.m13 = d;

        result.m21 = af + be * d;
        result.m22 = ae - bf * d;
        result.m23 = -b * c;

        result.m31 = bf - ae * d;
        result.m32 = be + af * d;
        result.m33 = a * c;

        return result;
    }
}
