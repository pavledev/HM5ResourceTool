#include "pch.h"
#include "Glacier2/ZObjectRef.hpp"

namespace IOI
{
	ZObjectRef::ZObjectRef()
	{
		m_TypeID = nullptr;
		m_pData = nullptr;
	}

	/*ZObjectRef::~ZObjectRef()
	{
		if (m_TypeID)
		{
			operator delete(m_TypeID);
		}

		if (m_pData)
		{
			operator delete(m_pData);
		}
	}*/

	STypeID* ZObjectRef::GetTypeID() const
	{
		return m_TypeID;
	}

	void* ZObjectRef::GetData() const
	{
		return m_pData;
	}

	void ZObjectRef::SetTypeID(STypeID* typeID)
	{
		m_TypeID = typeID;
	}

	void ZObjectRef::SetData(void* data)
	{
		m_pData = data;
	}
}
