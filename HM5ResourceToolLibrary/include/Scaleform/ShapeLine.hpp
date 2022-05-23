#pragma once

#include "ShapeLineType.hpp"

struct ShapeLine
{
    ShapeLineType type;
    int x, y;
    int sx, sy; //Only if type == splineTo
    int fillstyle0;
    int fillstyle1;
    int lineStyle;
    ShapeLine* next;
};
