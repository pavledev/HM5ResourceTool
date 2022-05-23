#include "pch.h"
#include <format>
#include "ZAudio.hpp"

namespace HM5ResourceTool
{
	void ZAudio::ConvertFSBFToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size)
	{
		binaryReader.GetInputStream()->Seek(16, IO::IInputStream::ESeekOrigin::Begin);

		*fsb5Data = binaryReader.GetInputStream()->GetCurrentPointer();
		fsb5Size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetSize()) - 16;
	}

	bool ZAudio::ConvertFSBFToOGG(const std::string& fsbfPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbfPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBFToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBFToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBFToWAV(const std::string& fsbfPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbfPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBFToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBFToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBFToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	void ZAudio::ConvertFSBMToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size)
	{
		binaryReader.GetInputStream()->Seek(24, IO::IInputStream::ESeekOrigin::Begin);

		*fsb5Data = binaryReader.GetInputStream()->GetCurrentPointer();
		fsb5Size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetSize()) - 24;
	}

	bool ZAudio::ConvertFSBMToOGG(const std::string& fsbmPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbmPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBMToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBMToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBMToWAV(const std::string& fsbmPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbmPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBMToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBMToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBMToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	void ZAudio::ConvertFSBSToFSB5(IO::ZBinaryReader& binaryReader, void** fsb5Data, unsigned int& fsb5Size)
	{
		unsigned int offset = FindFSB5DataOffsetInFSBS(binaryReader);

		if (offset == -1)
		{
			throw std::invalid_argument("File doesn't contain FSB5 data.");
		}

		binaryReader.GetInputStream()->Seek(offset, IO::IInputStream::ESeekOrigin::Begin);

		*fsb5Data = binaryReader.GetInputStream()->GetCurrentPointer();
		fsb5Size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetSize()) - offset;
	}

	bool ZAudio::ConvertFSBSToOGG(const std::string& fsbsPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbsPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBSToOGG(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBSToOGG(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToOGG(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBSToWAV(const std::string& fsbsPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsbsPath);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBSToWAV(const void* data, unsigned int dataSize, std::vector<SAudioSample*>& audioSamples)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, audioSamples);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSBSToWAV(const void* data, unsigned int dataSize, const std::string outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			void* fsb5Data = nullptr;
			unsigned int fsb5Size = 0;

			ConvertFSBSToFSB5(binaryReader, &fsb5Data, fsb5Size);
			ConvertFSB5ToWAV(fsb5Data, fsb5Size, outputFilePath);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	void ZAudio::ConvertFSB5ToOGG(void* fsb5Data, unsigned int& fsb5Size, const std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Data, fsb5Size);
		FSB::ZContainer container = FSB::ZContainer(binaryReader);
		FSB::Header header = container.FileHeader();
		std::size_t sampleNumber = 0;

		for (FSB::Sample sample : container.Samples())
		{
			std::string path = std::format("{}_{}", outputFilePath, sampleNumber);

			if (header.mode == FSB::Format::vorbis)
			{
				path += ".ogg";
			}
			else
			{
				path += ".wav";
			}

			std::ofstream output(path, std::ios_base::out | std::ios_base::binary);
			std::string message = std::format("Failed to open output file: {}", path);

			if (!output.is_open())
			{
				std::cout << message;

				return;
			}

			container.ExtractSample(sample, output);
			output.close();

			++sampleNumber;
		}
	}

	void ZAudio::ConvertFSB5ToOGG(void* fsb5Data, unsigned int& fsb5Size, std::vector<SAudioSample*>& audioSamples)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Data, fsb5Size);
		FSB::ZContainer container = FSB::ZContainer(binaryReader);

		for (FSB::Sample sample : container.Samples())
		{
			void* data = operator new(sample.size);
			IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(data, sample.size);

			container.ExtractSample(sample, binaryWriter);

			SAudioSample* audioSample = new SAudioSample();

			audioSample->data = binaryWriter.GetOutputStream()->GetBuffer();
			audioSample->dataSize = static_cast<unsigned int>(binaryWriter.GetOutputStream()->GetPosition());
			audioSample->name = sample.name;

			audioSamples.push_back(audioSample);
		}
	}

	void ZAudio::ConvertFSB5ToWAV(void* fsb5Data, unsigned int& fsb5Size, const std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Data, fsb5Size);
		FSB::ZContainer container = FSB::ZContainer(binaryReader);
		FSB::Header header = container.FileHeader();
		std::size_t sampleNumber = 0;

		for (FSB::Sample sample : container.Samples())
		{
			std::string path = std::format("{}_{}", outputFilePath, sampleNumber);

			if (header.mode == FSB::Format::vorbis)
			{
				path += ".ogg";
			}
			else
			{
				path += ".wav";
			}

			std::ofstream output(path, std::ios_base::out | std::ios_base::binary);
			std::string message = std::format("Failed to open output file: {}", path);

			if (!output.is_open())
			{
				std::cout << message;

				return;
			}

			container.ExtractSample(sample, output);
			output.close();

			++sampleNumber;
		}
	}

	void ZAudio::ConvertFSB5ToWAV(void* fsb5Data, unsigned int& fsb5Size, std::vector<SAudioSample*>& audioSamples)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Data, fsb5Size);
		FSB::ZContainer container = FSB::ZContainer(binaryReader);

		for (auto& sample : container.Samples())
		{
			void* data = operator new(sample.size);
			IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(data, sample.size);

			container.ExtractSample(sample, binaryWriter);

			SAudioSample* audioSample = new SAudioSample();

			audioSample->data = binaryWriter.GetOutputStream()->GetBuffer();
			audioSample->dataSize = static_cast<unsigned int>(binaryWriter.GetOutputStream()->GetPosition());
			audioSample->name = sample.name;

			audioSamples.push_back(audioSample);
		}
	}

	bool ZAudio::ConvertFSB5ToFSBF(const std::string& fsb5Path)
	{
		try
		{
			std::string fsbfPath = std::format("{}.FSBF", fsb5Path.substr(0, fsb5Path.find_last_of('.')));
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Path);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition());

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::Begin);

			IO::ZBinaryWriter binaryWriter(fsbfPath);

			for (int i = 0; i < 16; i++)
			{
				binaryWriter.WriteByte(0);
			}

			binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetBuffer(), size);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSB5ToFSBM(const std::string& fsb5Path)
	{
		try
		{
			std::string fsbmPath = std::format("{}.FSBM", fsb5Path.substr(0, fsb5Path.find_last_of('.')));
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Path);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition());

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::Begin);

			IO::ZBinaryWriter binaryWriter(fsbmPath);

			binaryWriter.WriteUInt32(0x4D425346); //MBSF
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(size);
			binaryWriter.WriteUInt32(0xFFFFFFFF);
			binaryWriter.WriteUInt32(0xFFFFFFFF);

			binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetBuffer(), size);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertFSB5ToFSBS(const std::string& fsb5Path)
	{
		try
		{
			std::string fsbsPath = std::format("{}.FSBS", fsb5Path.substr(0, fsb5Path.find_last_of('.')));
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(fsb5Path);

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition());

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::Begin);

			IO::ZBinaryWriter binaryWriter(fsbsPath);

			binaryWriter.WriteUInt32(0x4D425346); //SBSF
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(0);
			binaryWriter.WriteUInt32(size);
			binaryWriter.WriteUInt32(0xFFFFFFFF);
			binaryWriter.WriteUInt32(0xFFFFFFFF);

			for (int i = 0; i < 16; i++)
			{
				binaryWriter.WriteByte(0);
			}

			binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetBuffer(), size);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertWAVBToJSON(const std::string& wavbPath, std::string& jsonOutput)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(wavbPath);
			IOI::SWaveBankResourceData* waveBankResourceData = static_cast<IOI::SWaveBankResourceData*>(binaryReader.GetInputStream()->GetBuffer());

			jsonOutput = waveBankResourceData->SerializeToJSON();

			operator delete(waveBankResourceData);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertWAVBToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
			IOI::SWaveBankResourceData* waveBankResourceData = static_cast<IOI::SWaveBankResourceData*>(binaryReader.GetInputStream()->GetBuffer());

			jsonOutput = waveBankResourceData->SerializeToJSON();

			operator delete(waveBankResourceData);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertMUCBToJSON(const std::string& mucbPath, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SMusicCompositionData* musicCompositionData = static_cast<IOI::SMusicCompositionData*>(binaryDeserializer.Deserialize(mucbPath));
			jsonOutput = musicCompositionData->SerializeToJSON();

			operator delete(musicCompositionData);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertMUCBToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SMusicCompositionData* musicCompositionData = static_cast<IOI::SMusicCompositionData*>(binaryDeserializer.Deserialize(data, dataSize));
			jsonOutput = musicCompositionData->SerializeToJSON();

			operator delete(musicCompositionData);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::ConvertJSONToWAVB(const std::string& jsonPath)
	{
		try
		{
			IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(jsonPath);
			IOI::SWaveBankResourceData* waveBankResourceData = jsonDeserializer.Deserialize<IOI::SWaveBankResourceData>();

			delete waveBankResourceData;
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZAudio::IsOGGFormat(const void* data, const unsigned int dataSize, unsigned int startOffset)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

		return IsOGGFormat(binaryReader, startOffset);
	}

	bool ZAudio::IsOGGFormat(IO::ZBinaryReader& binaryReader, unsigned int startOffset)
	{
		unsigned int offset = offsetof(FSB::Header, mode);

		binaryReader.GetInputStream()->Seek(startOffset + offset, IO::IInputStream::ESeekOrigin::Begin);

		FSB::Format format = *static_cast<FSB::Format*>(binaryReader.ReadBytes(sizeof(FSB::Format)));

		if (format == FSB::Format::vorbis)
		{
			return true;
		}

		return false;
	}

	unsigned int ZAudio::FindFSB5DataOffsetInFSBS(IO::ZBinaryReader& binaryReader)
	{
		binaryReader.GetInputStream()->Seek(24 + 16, IO::IInputStream::ESeekOrigin::Begin);

		char* chars = binaryReader.ReadChars(4);
		std::string magic = std::string(chars, 4);
		unsigned int offset = -1;

		delete[] chars;

		if (magic == "FSB5")
		{
			offset = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition()) - 4;
		}
		else
		{
			binaryReader.GetInputStream()->Seek(36, IO::IInputStream::ESeekOrigin::Begin);

			chars = binaryReader.ReadChars(4);
			magic = std::string(chars, 4);

			delete[] chars;

			if (magic == "FACE")
			{
				binaryReader.GetInputStream()->Seek(24, IO::IInputStream::ESeekOrigin::Begin);

				unsigned int size = binaryReader.ReadUInt32();

				binaryReader.GetInputStream()->Seek(binaryReader.GetInputStream()->GetPosition() - 4 + size, IO::IInputStream::ESeekOrigin::Begin);

				while (true)
				{
					if (binaryReader.ReadByte() == 'F')
					{
						binaryReader.GetInputStream()->Seek(binaryReader.GetInputStream()->GetPosition() - 1, IO::IInputStream::ESeekOrigin::Begin);

						chars = binaryReader.ReadChars(4);
						magic = std::string(chars, 4);

						delete[] chars;

						if (magic == "FSB5")
						{
							break;
						}
					}
				}

				offset = static_cast<unsigned int>(binaryReader.GetInputStream()->GetPosition()) - 4;
			}
		}

		return offset;
	}
}
