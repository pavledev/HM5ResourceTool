#pragma once

namespace IOI
{
	template <typename TElement, int TSize>
	struct ZFixedArrayData
	{
		TElement m_pStart[TSize];
	};
}
