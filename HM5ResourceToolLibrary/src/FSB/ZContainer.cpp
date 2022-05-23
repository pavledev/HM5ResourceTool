#include "pch.h"
#include <format>
#include "FSB/ZContainer.hpp"

FSB::ZContainer::ZContainer(std::istream& stream)
{
    ReadFileHeader(stream);
    ReadSampleHeaders(stream);
    ReadSampleNames(stream);

    dataBuffer = IO::ZUtility::Read(stream, header.dataSize);
}

FSB::ZContainer::ZContainer(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
{
    ReadFileHeader(binaryReader);
    ReadSampleHeaders(binaryReader);
    ReadSampleNames(binaryReader);

    dataBuffer = IO::ZUtility::Read(binaryReader, header.dataSize);
}

const FSB::Header& FSB::ZContainer::FileHeader() const
{
    return header;
}

const std::vector<FSB::Sample>& FSB::ZContainer::Samples() const
{
    return samples;
}

void FSB::ZContainer::ExtractSample(const Sample& sample, std::ostream& stream)
{
    if (header.mode != Format::vorbis && header.mode != Format::imaadpcm)
    {
        throw std::invalid_argument("Unsupported file type.");
    }

    // Construct sample data view to verify that we don't exceed sample boundaries 
    // during extraction process.
    if (sample.offset > dataBuffer.size())
    {
        throw std::invalid_argument("Offset is greater then size of buffer.");
    }

    if (sample.offset + sample.size > dataBuffer.size())
    {
        throw std::invalid_argument("Offset with added size is greater then size of buffer.");
    }

    const char* const sampleBegin = dataBuffer.data() + sample.offset;
    const char* const sampleEnd = sampleBegin + sample.size;
    IO::ZBufferView sampleView(sampleBegin, sampleEnd);

    if (header.mode == Format::vorbis)
    {
        Vorbis::ZRebuilder rebuilder;

        rebuilder.Rebuild(sample, sampleView, stream);
    }
    else
    {
        IO::ZWAVWriter::WriteToFile(sample, sampleBegin, stream);
    }
}

void FSB::ZContainer::ExtractSample(const Sample& sample, HM5ResourceTool::IO::ZBinaryWriter& binaryWriter)
{
    if (header.mode != Format::vorbis && header.mode != Format::imaadpcm)
    {
        throw std::invalid_argument("Unsupported file type.");
    }

    // Construct sample data view to verify that we don't exceed sample boundaries 
    // during extraction process.
    if (sample.offset > dataBuffer.size())
    {
        throw std::invalid_argument("Offset is greater then size of buffer.");
    }

    if (sample.offset + sample.size > dataBuffer.size())
    {
        throw std::invalid_argument("Offset with added size is greater then size of buffer.");
    }

    char* sampleBegin = dataBuffer.data() + sample.offset;
    char* sampleEnd = sampleBegin + sample.size;
    IO::ZBufferView sampleView(sampleBegin, sampleEnd);

    if (header.mode == Format::vorbis)
    {
        Vorbis::ZRebuilder rebuilder;

        rebuilder.Rebuild(sample, sampleView, binaryWriter);
    }
    else
    {
        IO::ZWAVWriter::WriteToFile(sample, sampleBegin, binaryWriter);
    }
}

void FSB::ZContainer::ReadFileHeader(std::istream& stream)
{
    const std::vector<char> headerBuffer = IO::ZUtility::Read(stream, headerSize);

    ReadFileHeader(headerBuffer);
}

void FSB::ZContainer::ReadFileHeader(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
{
    const std::vector<char> headerBuffer = IO::ZUtility::Read(binaryReader, headerSize);

    ReadFileHeader(headerBuffer);
}

void FSB::ZContainer::ReadFileHeader(const std::vector<char>& headerBuffer)
{
    IO::ZBufferView headerView(headerBuffer.data(), headerBuffer.size());

    header.id[0] = headerView.ReadChar();
    header.id[1] = headerView.ReadChar();
    header.id[2] = headerView.ReadChar();
    header.id[3] = headerView.ReadChar();

    if (!std::equal(header.id, header.id + 4, "FSB5"))
    {
        throw std::invalid_argument("Unsupported file version.");
    }

    header.version = headerView.ReadUInt32();

    if (header.version != 1)
    {
        throw std::invalid_argument("Version must be 1.");
    }

    header.samples = headerView.ReadUInt32();
    header.headersSize = headerView.ReadUInt32();
    header.namesSize = headerView.ReadUInt32();
    header.dataSize = headerView.ReadUInt32();

    const std::uint32_t mode = headerView.ReadUInt32();
    std::string message = std::format("Mode field is out of range: {}", mode);

    if (mode <= 0 || mode >= static_cast<std::uint32_t>(Format::max))
    {
        throw std::invalid_argument(message);
    }

    header.mode = static_cast<Format>(mode);
    header.unknown = headerView.ReadUInt64();

    for (int i = 0; i < 24; ++i)
    {
        header.guid[i] = headerView.ReadUInt8();
    }
}

void FSB::ZContainer::ReadSampleHeaders(std::istream& stream)
{
    const std::vector<char> samplesBuffer = IO::ZUtility::Read(stream, header.headersSize);

    ReadSampleHeaders(samplesBuffer);
}

void FSB::ZContainer::ReadSampleHeaders(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
{
    const std::vector<char> samplesBuffer = IO::ZUtility::Read(binaryReader, header.headersSize);

    ReadSampleHeaders(samplesBuffer);
}

void FSB::ZContainer::ReadSampleHeaders(const std::vector<char>& samplesBuffer)
{
    IO::ZBufferView samplesView(samplesBuffer.data(), samplesBuffer.size());

    for (std::uint32_t i = 0; i < header.samples; ++i)
    {
        samples.push_back(ReadSampleHeader(samplesView));
    }

    // Calculate sample sizes, assuming that samples are laid out contiguously.
    const Sample* nextSample{};

    for (int i = samples.size() - 1; i >= 0; i--)
    {
        const auto nextOffset = nextSample ? nextSample->offset : header.dataSize;

        if (nextOffset < samples[i].offset)
        {
            throw std::invalid_argument("Offset of next sample is smaller then offset of current sample.");
        }

        samples[i].size = nextOffset - samples[i].offset;
        nextSample = &samples[i];
    }
}

FSB::Sample FSB::ZContainer::ReadSampleHeader(IO::ZBufferView& view)
{
    Sample sample;

    // Bits:
    // 0       has more flags
    // 1 2 3 4 frequency
    // 5 6     log2(channels)
    // 7       first bit of sample offset
    const std::uint8_t mode = view.ReadUInt8();
    bool hasExtraHeaders = mode & 1u;
    sample.channels = 1u << ((mode & 96u) >> 5u);

    switch ((mode & 31u) >> 1u)
    {
    case 1u: sample.frequency = 8000u; break;
    case 2u: sample.frequency = 11000u; break;
    case 3u: sample.frequency = 11025u; break;
    case 4u: sample.frequency = 16000u; break;
    case 5u: sample.frequency = 22050u; break;
    case 6u: sample.frequency = 24000u; break;
    case 7u: sample.frequency = 32000u; break;
    case 8u: sample.frequency = 44100u; break;
    case 9u: sample.frequency = 48000u; break;
    default: sample.frequency = 44100u; break;
    }

    const std::size_t sampleOffsetBit0 = mode >> 7u;
    const std::size_t sampleOffsetBit1Plus = view.ReadUInt24();
    // Samples have 32 byte alignment.
    sample.offset = (sampleOffsetBit0 | sampleOffsetBit1Plus << 1u) << 5u;

    // ???
    //sample.unknown = view.ReadUInt32();
    sample.samples = view.ReadUInt32() >> 2;

    while (hasExtraHeaders)
    {
        // Bits:
        // 0        has extra headers
        // 1 ... 23 length
        const uint32_t extra = view.ReadUInt24();
        hasExtraHeaders = extra & 0x01;
        std::size_t extraLength = extra >> 1u;
        const uint8_t type = view.ReadUInt8();

        switch (type)
        {
        case 0x02:
        {
            if (extraLength != 1)
            {
                throw std::invalid_argument("extraLength should be 1.");
            }

            sample.channels = view.ReadUInt8();
            break;
        }
        case 0x04:
        {
            if (extraLength != 4)
            {
                throw std::invalid_argument("extraLength should be 4.");
            }

            sample.frequency = view.ReadUInt32();
            break;
        }
        case 0x06:
        {
            if (extraLength != 8)
            {
                throw std::invalid_argument("extraLength should be 8.");
            }

            sample.loopStart = view.ReadUInt32();
            sample.loopEnd = view.ReadUInt32();
            break;
        }
        case 0x16:
        {
            if (extraLength < 4)
            {
                throw std::invalid_argument("extraLength should be greater then or equal to 4.");
            }

            sample.vorbisCRC32 = view.ReadUInt32();
            extraLength -= 4;
            // Maybe position / seek information (granulepos)?
            if (extraLength % 4 != 0)
            {
                throw std::invalid_argument("extraLength % 4 should be 0.");
            }
            // Seek information. Two sequences non-decreasing sequences of 32 bit
            // numbers laid out as follows:
            // a_1, b_1, a_2, b_2, ..., a_n-1, b_n-1, a_n
            // Where a_i are offsets within sample audio data (pointing at beginning
            // of packets), and b_i are associated granulepos.
            // But there are granulepos only for some packets, not for all of them.
            view.Skip(extraLength);
            break;
        }
        default:
        {
            throw std::invalid_argument(std::format("Unexpected extra header type: {}", type));

            view.Skip(extraLength);
        }
        }
    }

    return sample;
}

void FSB::ZContainer::ReadSampleNames(std::istream& stream)
{
    const std::vector<char> namesBuffer = IO::ZUtility::Read(stream, header.namesSize);

    ReadSampleNames(namesBuffer);
}

void FSB::ZContainer::ReadSampleNames(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
{
    const std::vector<char> namesBuffer = IO::ZUtility::Read(binaryReader, header.namesSize);

    ReadSampleNames(namesBuffer);
}

void FSB::ZContainer::ReadSampleNames(const std::vector<char>& namesBuffer)
{
    IO::ZBufferView namesView(namesBuffer.data(), namesBuffer.size());

    for (std::uint32_t i = 0; i < header.samples; ++i)
    {
        if (namesView.Empty())
        {
            samples[i].name = std::format("Sample {}", i);
        }
        else
        {
            namesView.SetOffset(i * 4u);
            namesView.SetOffset(namesView.ReadUInt32());

            while (char c = namesView.ReadChar())
            {
                samples[i].name.push_back(c);
            }
        }
    }
}
