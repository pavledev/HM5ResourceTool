#pragma once

#include "./TypeInfo/STypeID.hpp"

namespace IOI
{
	class ZObjectRef
	{
	public:
		ZObjectRef();
		//virtual ~ZObjectRef();
		STypeID* GetTypeID() const;
		void* GetData() const;
		void SetTypeID(STypeID* typeID);
		void SetData(void* data);

	protected:
		STypeID* m_TypeID;
		void* m_pData;
	};
}
