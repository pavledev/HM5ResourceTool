#include "pch.h"
#include <limits>
#include <istream>
#include "FSB/IO/ZUtil.hpp"

#undef max

namespace FSB
{
    namespace IO
    {
        void ZUtility::Read(std::istream& stream, char* buffer, std::size_t size)
        {
            constexpr std::streamsize streamSizeMax = std::numeric_limits<std::streamsize>::max();

            while (size > 0)
            {
                const std::streamsize toRead = streamSizeMax < size ? streamSizeMax : size;

                stream.read(buffer, toRead);

                size -= static_cast<size_t>(toRead);
            }
        }

        std::vector<char> ZUtility::Read(std::istream& stream, std::size_t size)
        {
            std::vector<char> buffer(size);

            Read(stream, buffer.data(), buffer.size());

            return buffer;
        }

        void ZUtility::Read(HM5ResourceTool::IO::ZBinaryReader& binaryReader, char* buffer, std::size_t size)
        {
            constexpr std::streamsize streamSizeMax = std::numeric_limits<std::streamsize>::max();

            while (size > 0)
            {
                const std::streamsize toRead = streamSizeMax < size ? streamSizeMax : size;

                binaryReader.ReadBytes(buffer, static_cast<size_t>(toRead));

                size -= static_cast<size_t>(toRead);
            }
        }

        std::vector<char> ZUtility::Read(HM5ResourceTool::IO::ZBinaryReader& binaryReader, std::size_t size)
        {
            std::vector<char> buffer(size);

            Read(binaryReader, buffer.data(), buffer.size());

            return buffer;
        }
    }
}
