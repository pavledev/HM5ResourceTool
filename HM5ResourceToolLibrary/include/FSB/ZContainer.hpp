#pragma once

#include <istream>
#include <vector>
#include "IO/ZUtil.hpp"
#include "IO/ZBufferView.hpp"
#include "FSB.hpp"
#include "Vorbis/Vorbis.hpp"
#include "Vorbis/ZRebuilder.hpp"
#include "IO/ZWAVWriter.hpp"
#include "IO/ZBinaryWriter.hpp"

namespace FSB
{
	class ZContainer
	{
	public:
		ZContainer(std::istream& stream);
		ZContainer(HM5ResourceTool::IO::ZBinaryReader& binaryReader);

		const Header& FileHeader() const;

		const std::vector<Sample>& Samples() const;

		// Extracts sample audio data to given stream.
		void ExtractSample(const Sample& sample, std::ostream& stream);
		void ExtractSample(const Sample& sample, HM5ResourceTool::IO::ZBinaryWriter& binaryWriter);

	private:
		ZContainer(const ZContainer& rhs) = delete;
		ZContainer& operator=(const ZContainer& rhs) = delete;

		// Reads file header from a stream.
		void ReadFileHeader(std::istream& stream);
		void ReadFileHeader(HM5ResourceTool::IO::ZBinaryReader& binaryReader);
		void ReadFileHeader(const std::vector<char>& headerBuffer);

		// Reads sample headers from a stream.
		void ReadSampleHeaders(std::istream& stream);
		void ReadSampleHeaders(HM5ResourceTool::IO::ZBinaryReader& binaryReader);
		void ReadSampleHeaders(const std::vector<char>& samplesBuffer);

		// Reads a single sample header from a view.
		// 
		// Position view on a next sample header.
		Sample ReadSampleHeader(IO::ZBufferView& view);

		// Reads sample names.
		void ReadSampleNames(std::istream& stream);
		void ReadSampleNames(HM5ResourceTool::IO::ZBinaryReader& binaryReader);
		void ReadSampleNames(const std::vector<char>& namesBuffer);

		static const int headerSize = 60;
		Header header;
		std::vector<Sample> samples;
		std::vector<char> dataBuffer;
	};
}
