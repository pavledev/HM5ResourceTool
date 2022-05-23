#pragma once

#include <vector>
#include "SResourceHeaderHeader.hpp"
#include "ZRuntimeResourceID.hpp"
#include "EResourceReferenceFlags.hpp"

namespace IOI
{
	class ZResourceHeaderReader
	{
	public:
		SResourceHeaderHeader m_HeaderHeader;
		const unsigned char* m_pReferencesChunk;

		ZResourceHeaderReader(SResourceHeaderHeader headerHeader, const unsigned char* pReferencesChunk);
		unsigned int GetResourceType() const;
		unsigned int GetNumResourceIdentifiers() const;
		ZRuntimeResourceID GetResourceIdentifier(unsigned int lResourceIdentifierIndex) const;
		EResourceReferenceFlags GetResourceFlags(unsigned int lResourceIdentifierIndex) const;
		bool HasIntegerFormat() const;
	};
}
