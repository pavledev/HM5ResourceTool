#pragma once

namespace HM5ResourceToolCLI
{
	public enum class EResourceReferenceFlags : unsigned int
	{
		NoFlags = 0x0,
		OffsetBits = 0xFFFFFF,
		BitCount = 0x18,
		FlagBits = 0xFF000000,
		TypeOfStreamingEntity = 0x10000000,
		StateStreamed = 0x20000000,
		MediaStreamed = 0x40000000,
		InstallDependency = 0x80000000,
		Cyclic = 0x4000000,
	};
}
