#pragma once

struct SwfLayout;
struct SwfGlyph;
struct AlignZone;
struct FontUsage;

struct SwfFont
{
    int id; //-1 = not set
    unsigned char version; //0 = not set, 1 = definefont, 2 = definefont2, 3 = definefont3
    unsigned char* name;
    SwfLayout* layout;
    int numberOfChars;
    int maxAscii; //Highest mapped ascii/unicode value

    unsigned char style;
    unsigned char encoding;

    unsigned short* glyph2Ascii;
    int* ascii2Glyph;
    int* glyph2Glyph; //Only if the font is resorted
    SwfGlyph* glyph;
    AlignZone* alignZones;
    unsigned char alignzoneFlags;
    unsigned char language;
    char** glyphNames;

    FontUsage* use;
};
