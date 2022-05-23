#pragma once

#include <cstdint>
#include <iosfwd>
#include "../FSB.hpp"
#include "../IO/ZBufferView.hpp"
#include "Vorbis.hpp"
#include "IO/ZBinaryWriter.hpp"

namespace FSB
{
    namespace Vorbis
    {
        // Rebuilds Vorbis headers and audio data.
        class ZRebuilder
        {
        public:
            ZRebuilder();

            // Rebuilds sample and write it to a stream.
            static void Rebuild(const Sample& sample, IO::ZBufferView& sampleView, std::ostream& stream);
            static void Rebuild(const Sample& sample, IO::ZBufferView& sampleView, HM5ResourceTool::IO::ZBinaryWriter& binaryWriter);

            // Rebuilds Vorbis headers and returns them as Ogg packets.
            static void RebuildHeaders(
                int channels, int rate, std::uint32_t crc32,
                std::uint32_t loopStart, std::uint32_t loopEnd,
                OGGPacketHolder& id,
                OGGPacketHolder& comment,
                OGGPacketHolder& setup);

            // Rebuilds identification header.
            static void RebuildIDHeader(
                int channels, int rate, int blocksizeShort, int blocksizeLong,
                OGGPacketHolder& packet);

            // Rebuilds comment header.
            static void RebuildCommentHeader(
                OGGPacketHolder& packet,
                std::uint32_t loopStart, std::uint32_t loopEnd);

            // Rebuilds setup header.
            static void RebuildSetupHeader(
                const char* payload, std::size_t payloadSize,
                OGGPacketHolder& packet);

        private:
            ZRebuilder(const ZRebuilder& rhs) = delete;
            ZRebuilder& operator=(const ZRebuilder& rhs) = delete;
        };
    }
}
