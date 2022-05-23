#pragma once

#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/Glacier2/Resource/ZRuntimeResourceID.hpp"

namespace HM5ResourceToolCLI
{
	public ref class ZRuntimeResourceID
	{
	public:
		ZRuntimeResourceID();
		ZRuntimeResourceID(IOI::ZRuntimeResourceID* runtimeResourceID);
		unsigned long long GetID();
		static ZRuntimeResourceID^ Create(unsigned long long nResource);
		bool IsLibraryResource();
		int GetIndexInLibrary();
		void SetHigh(unsigned int idHigh);
		void SetLow(unsigned int idLow);

	private:
		IOI::ZRuntimeResourceID* instance;
	};
}
