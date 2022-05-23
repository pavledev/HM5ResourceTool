#include "pch.h"
#include <string>
#include "Glacier2/TypeInfo/IType.hpp"

namespace IOI
{
	IType::IType(unsigned short m_nTypeSize, unsigned char m_nTypeAlignment, unsigned char m_nTypeInfoFlags, const char* pszTypeName)
	{
		const size_t length = strlen(pszTypeName);
		char* typeName = new char[length + 1];

		strncpy_s(typeName, length + 1, pszTypeName, length + 1);

		this->m_pTypeFunctions = nullptr;
		this->m_nTypeSize = m_nTypeSize;
		this->m_nTypeAlignment = m_nTypeAlignment;
		this->m_nTypeInfoFlags = m_nTypeInfoFlags;
		this->pszTypeName = typeName;
		this->typeID = nullptr;
		this->fromString = nullptr;
		this->toString = nullptr;
	}

	IType::~IType()
	{
		delete[] this->pszTypeName;
	}

	unsigned int IType::GetTypeSize() const
	{
		return m_nTypeSize;
	}

	unsigned int IType::GetTypeAlignment() const
	{
		return m_nTypeAlignment;
	}

	ETypeInfoFlags IType::GetTypeInfoFlags() const
	{
		return static_cast<ETypeInfoFlags>(m_nTypeInfoFlags);
	}

	bool IType::IsEnumType() const
	{
		return GetTypeInfoFlags() & ETypeInfoFlags::ETYPEINFOFLAG_ENUMTYPE;
	}

	bool IType::IsPrimitiveType() const
	{
		std::string typeName = pszTypeName;

		return m_nTypeInfoFlags == 0 && (
			typeName == "bool" ||
			typeName == "char" ||
			typeName == "int8" ||
			typeName == "uint8" ||
			typeName == "int16" ||
			typeName == "uint16" ||
			typeName == "int32" ||
			typeName == "uint32" ||
			typeName == "int64" ||
			typeName == "uint64" ||
			typeName == "float32" ||
			typeName == "float64");
	}
}
