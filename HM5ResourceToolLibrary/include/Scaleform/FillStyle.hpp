#pragma once

#include "RGBA.hpp"
#include "Matrix.hpp"
#include "Gradient.hpp"

struct FillStyle
{
    unsigned char type;
    RGBA color;
    Matrix m;
    unsigned short idBitmap;
    Gradient gradient;
};
