#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZAudio.hpp"
#include "SAudioSample.hpp"

using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZAudio : public ManagedObject<HM5ResourceTool::ZAudio>
    {
    public:
        ZAudio();
        static bool ConvertFSBFToOGG(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertFSBFToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertFSBMToOGG(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertFSBMToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertFSBSToOGG(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertFSBSToWAV(const array<Byte>^ data, unsigned int dataSize, [Out] List<SAudioSample^>^% audioSamples);
        static bool ConvertWAVBToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        static bool ConvertMUCBToJSON(const array<Byte>^ data, const unsigned int dataSize, [Out] String^% jsonOutput);
        static bool IsOGGFormat(const array<Byte>^ data, const unsigned int dataSize, unsigned int startOffset);
    };
}
