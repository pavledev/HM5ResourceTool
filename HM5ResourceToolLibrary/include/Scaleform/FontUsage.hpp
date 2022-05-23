#pragma once

struct SwfGlyphPair;

struct FontUsage
{
    int* chars;
    char isReduced;
    int usedGlyphs;
    int glyphsSpecified;
    unsigned short smallestSize;

    SwfGlyphPair* neighbors;
    int numberOfNeighbors;
    int neighborsSize;
    int* neighborsHash;
    int neighborsHashSize;
};
