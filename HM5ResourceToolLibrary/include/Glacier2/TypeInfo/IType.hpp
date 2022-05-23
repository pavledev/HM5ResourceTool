#pragma once

#include <string.h>
#include "../ZString.hpp"
#include "ETypeInfoFlags.hpp"

namespace IOI
{
	struct STypeID;
	struct STypeFunctions;

	class IType
	{
	public:
		const STypeFunctions* m_pTypeFunctions;
		unsigned short m_nTypeSize;
		unsigned char m_nTypeAlignment;
		unsigned char m_nTypeInfoFlags;
		const char* pszTypeName;
		STypeID* typeID;
		bool (*fromString)(void* param1, const IType* type, const ZString& string);
		unsigned int (*toString)(const void* param1, const IType* type, char* param3, unsigned int param4, const ZString& string);

		IType(unsigned short m_nTypeSize, unsigned char m_nTypeAlignment, unsigned char m_nTypeInfoFlags, const char* pszTypeName);
		~IType();
		unsigned int GetTypeSize() const;
		unsigned int GetTypeAlignment() const;
		ETypeInfoFlags GetTypeInfoFlags() const;
		bool IsEnumType() const;
		bool IsPrimitiveType() const;
	};
}
