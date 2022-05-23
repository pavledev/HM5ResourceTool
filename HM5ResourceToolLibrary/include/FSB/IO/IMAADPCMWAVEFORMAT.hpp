#pragma once

#include "WAVEFORMATEX.hpp"

namespace FSB
{
    namespace IO
    {
        struct IMAADPCMWAVEFORMAT
        {
            WAVEFORMATEX waveFormatEX;
            unsigned short wSamplesPerBlock;
        };
    }
}
