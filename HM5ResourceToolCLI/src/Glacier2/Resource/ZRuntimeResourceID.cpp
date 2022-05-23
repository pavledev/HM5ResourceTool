#include "pch.h"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"

namespace HM5ResourceToolCLI
{
	ZRuntimeResourceID::ZRuntimeResourceID()
	{

	}

	ZRuntimeResourceID::ZRuntimeResourceID(IOI::ZRuntimeResourceID* runtimeResourceID)
	{
		this->instance = runtimeResourceID;
	}

	unsigned long long ZRuntimeResourceID::GetID()
	{
		return instance->GetID();
	}

	ZRuntimeResourceID^ ZRuntimeResourceID::Create(unsigned long long nResource)
	{
		ZRuntimeResourceID^ result = gcnew ZRuntimeResourceID();
		IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::Create(nResource);

		result->SetHigh(runtimeResourceID.GetHigh());
		result->SetLow(runtimeResourceID.GetLow());

		return result;
	}

	bool ZRuntimeResourceID::IsLibraryResource()
	{
		return instance->IsLibraryResource();
	}

	int ZRuntimeResourceID::GetIndexInLibrary()
	{
		return instance->GetIndexInLibrary();
	}

	void ZRuntimeResourceID::SetHigh(unsigned int idHigh)
	{
		instance->SetHigh(idHigh);
	}

	void ZRuntimeResourceID::SetLow(unsigned int idLow)
	{
		instance->SetLow(idLow);
	}
}
