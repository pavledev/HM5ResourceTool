#pragma once

#include "SRECT.hpp"
#include "Tag.hpp"

struct Swf
{
    unsigned char fileVersion;
    unsigned char compressed; // SWF or SWC?
    unsigned int fileSize; // valid after load and save
    SRect movieSize;
    unsigned short frameRate;
    unsigned short frameCount; // valid after load and save
    Tag* firstTag;
    unsigned int fileAttributes; // for SWFs >= Flash9

    Swf() = default;
};
