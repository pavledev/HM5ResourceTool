#pragma once

struct SRect;
struct SwfKerning;

struct SwfLayout
{
    unsigned short ascent;
    unsigned short descent;
    unsigned short leading;
    SRect* bounds;
    unsigned short kerningCount;
    SwfKerning* kerning;
};
