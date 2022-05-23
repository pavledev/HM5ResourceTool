#pragma once

#include "Matrix.hpp"

struct Character;
struct CXForm;
struct RenderBuffer;

struct TextCallbackBlock
{
    Character* idtable;
    unsigned short depth;
    unsigned short clipdepth;
    CXForm* cxform;
    Matrix m;
    RenderBuffer* buf;
};
