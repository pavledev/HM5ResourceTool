#pragma once

struct LineStyle;
struct FillStyle;

struct Shape // NEVER access a Shape-Struct directly !
{
    struct
    {
        LineStyle* data;
        unsigned short n;
    } lineStyle;

    struct
    {
        FillStyle* data;
        unsigned short n;
    } fillStyle;

    struct
    {
        unsigned short fill;
        unsigned short line;
    } bits;

    //Used by Get/SetSimpleShape and glyph handling
    unsigned char* data;
    unsigned int bitLength; //Length of data in bits
};
