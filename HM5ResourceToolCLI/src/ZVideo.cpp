#include "pch.h"
#include "ZVideo.hpp"

namespace HM5ResourceToolCLI
{
	bool ZVideo::ConvertPCBinkVidToBINK(String^ pcBinkVidPath, array<Byte>^% binkData, unsigned int% binkSize)
	{
		std::string pcBinkVidPath2 = marshal_as<std::string>(pcBinkVidPath);
		void* binkData2 = nullptr;
		unsigned int binkSize2 = 0;

		bool result = HM5ResourceTool::ZVideo::ConvertPCBinkVidToBINK(pcBinkVidPath2, &binkData2, binkSize2);
		IntPtr binkDataPointer = IntPtr(binkData2);

		binkSize = binkSize2;
		binkData = gcnew array<Byte>(binkSize);

		Marshal::Copy(binkDataPointer, binkData, 0, binkSize);

		return result;
	}
}
