#pragma once

#include <cstdint>
#include <istream>
#include <string>

namespace FSB
{
	// Sample format
	enum class Format : unsigned int
	{
		none,     // Unitialized / unknown.
		pcm8,     // 8bit integer PCM data.
		pcm16,    // 16bit integer PCM data.
		pcm24,    // 24bit integer PCM data.
		pcm32,    // 32bit integer PCM data.
		pcmfloat, // 32bit floating point PCM data.
		gcadpcm,  // Compressed Nintendo 3DS/Wii DSP data.
		imaadpcm, // Compressed IMA ADPCM data.
		vag,      // Compressed PlayStation Portable ADPCM data.
		hevag,    // Compressed PSVita ADPCM data.
		xma,      // Compressed Xbox360 XMA data.
		mpeg,     // Compressed MPEG layer 2 or 3 data.
		celt,     // Compressed CELT data.
		at9,      // Compressed PSVita ATRAC9 data.
		xwma,     // Compressed Xbox360 xWMA data.
		vorbis,   // Compressed Vorbis data.
		max,      // Maximum number of sound formats supported.
	};

	// FSB file header.
	struct Header
	{
		// FSB file format identifier.
		char id[4];
		// File format version.
		unsigned int version = 0;
		// Number of samples in a file.
		unsigned int samples = 0;
		// Size of section with sample headers.
		unsigned int headersSize = 0;
		// Size of section with sample names.
		unsigned int namesSize = 0;
		// Size of section with sample data.
		unsigned int dataSize = 0;
		// Samples format.
		Format mode;
		// ?
		unsigned long long unknown;
		// Some kind of hash.
		std::int8_t guid[24];
	};

	// FSB sample.
	struct Sample
	{
		// Sample name.
		std::string name;
		// Sample rate.
		unsigned int frequency = 0;
		// Number of channels in sample.
		unsigned char channels = 0;
		// Position of sample expressed as an offset from start of data section.
		std::size_t offset = 0;
		// Size of sample in data section.
		std::size_t size = 0;
		// CRC-32 of Vorbis setup header.
		unsigned int vorbisCRC32 = 0;
		unsigned int loopStart = 0;
		unsigned int loopEnd = 0;
		//unsigned int unknown = 0;
		unsigned int samples = 0;
	};
}
