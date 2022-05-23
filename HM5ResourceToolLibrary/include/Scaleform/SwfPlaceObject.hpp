#pragma once

#include "Matrix.hpp"
#include "CXForm.hpp"

struct ActionTAG;
struct FilterList;

struct SwfPlaceObject
{
    unsigned char flags;
    unsigned short depth;
    unsigned short id; // may be 0
    char move; //true: move/replace character, false: set character
    Matrix Matrix;
    CXForm cxform;
    unsigned short ratio;
    char* name;
    unsigned short clipdepth;
    ActionTAG* actions;
    unsigned char blendmode;
    FilterList* filters;
};
