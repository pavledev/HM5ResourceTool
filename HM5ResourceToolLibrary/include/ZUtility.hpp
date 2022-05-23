#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Glacier2/Resource/EResourceReferenceFlags.hpp"

#undef max
#undef min

namespace IOI
{
	struct SVector4;
	struct SVector2;
}

struct SColorRGBA
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	SColorRGBA() = default;

	SColorRGBA(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

template <typename T>
struct SVector6
{
	T a, b, c, d, e, f;

	SVector6() = default;

	SVector6(const T a, const T b, const T c, const T d, const T e, const T f)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->e = e;
		this->f = f;
	};
};

struct SVertex
{
	IOI::SVector4* position;
	IOI::SVector4* normal;
	IOI::SVector4* tangent;
	IOI::SVector4* biTangent;
	std::vector<IOI::SVector2*> uv;
	SColorRGBA* color;
	SVector6<float> weight;
	SVector6<unsigned char> joint;

	SVertex() = default;
};

class ZUtility
{
public:
	static std::string ConvertTypeToString(unsigned int type);
	static unsigned int ConvertStringToType(const std::string& string);
	static std::string ConvertResourceReferenceFlagsToString(const IOI::EResourceReferenceFlags resourceReferenceFlags);
	static std::string GetBaseNameFromPath(const std::string& path);

	template <typename T>
	static std::string ConvertValueToString(const T value)
	{
		std::stringstream stringstream;

		stringstream << value;

		return stringstream.str();
	}

	template <>
	static std::string ConvertValueToString(const float value)
	{
		std::stringstream stringstream;

		stringstream << std::fixed << std::setprecision(10) << value;

		return stringstream.str();
	}

	template <typename T>
	static std::string ConvertValueToHexString(const T value)
	{
		std::stringstream stringstream;

		stringstream << "0x" << std::hex << std::uppercase << value;

		return stringstream.str();
	}

	template <typename T>
	static void AddValueToBuffer(std::vector<unsigned char>& buffer, const T value)
	{
		unsigned char const* pointer = reinterpret_cast<unsigned char const*>(&value);

		for (std::size_t i = 0; i != sizeof(T); ++i)
		{
			buffer.emplace_back(pointer[i]);
		}
	}

	template <typename T>
	static T Clamp(T value, T min, T max)
	{
		return std::max(min, std::min(max, value));
	}
};
