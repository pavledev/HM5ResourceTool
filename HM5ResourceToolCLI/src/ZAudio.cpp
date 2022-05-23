#include "pch.h"
#include "ZAudio.hpp"

namespace HM5ResourceToolCLI
{
	ZAudio::ZAudio() : ManagedObject(new HM5ResourceTool::ZAudio())
	{

	}

	bool ZAudio::ConvertFSBFToOGG(const array<Byte>^ data, unsigned int dataSize, List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBFToOGG(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertFSBFToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBFToWAV(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertFSBMToOGG(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBMToOGG(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertFSBMToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBFToWAV(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertFSBSToOGG(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBSToOGG(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertFSBSToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::vector<HM5ResourceTool::SAudioSample*> audioSamples2;

		bool result = HM5ResourceTool::ZAudio::ConvertFSBSToWAV(dataPointer, dataSize, audioSamples2);

		unsigned int count = audioSamples2.size();
		audioSamples = gcnew List<SAudioSample^>(count);

		for (unsigned int i = 0; i < count; ++i)
		{
			SAudioSample^ audioSample = gcnew SAudioSample(audioSamples2[i]);

			audioSamples->Add(audioSample);
		}

		return result;
	}

	bool ZAudio::ConvertWAVBToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZAudio::ConvertWAVBToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZAudio::ConvertMUCBToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZAudio::ConvertMUCBToJSON(dataPointer, dataSize, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}

	bool ZAudio::IsOGGFormat(const array<Byte>^ data, const unsigned int dataSize, unsigned int startOffset)
	{
		pin_ptr<System::Byte> dataPointer = &data[0];

		return HM5ResourceTool::ZAudio::IsOGGFormat(dataPointer, dataSize, startOffset);
	}
}
