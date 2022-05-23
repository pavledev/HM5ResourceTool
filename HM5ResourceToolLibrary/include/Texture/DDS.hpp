#pragma once

namespace DDS
{
	struct DDS_PIXELFORMAT
	{
		unsigned int size;
		unsigned int flags;
		unsigned int fourCC;
		unsigned int rgbBitCount;
		unsigned int rBitMask;
		unsigned int gBitMask;
		unsigned int bBitMask;
		unsigned int aBitMask;
	};

	struct DDS_HEADER
	{
		unsigned int magic;
		unsigned int size;
		unsigned int flags;
		unsigned int height;
		unsigned int width;
		unsigned int pitchOrLinearSize;
		unsigned int depth;
		unsigned int mipMapCount;
		unsigned int reserved1[11];
		DDS_PIXELFORMAT ddspf;
		unsigned int caps;
		unsigned int caps2;
		unsigned int caps3;
		unsigned int caps4;
		unsigned int reserved2;
	};
}
