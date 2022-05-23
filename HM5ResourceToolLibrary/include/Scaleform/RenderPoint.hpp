#pragma once

struct ShapeLine;
struct Shape2;

struct RenderPoint
{
    float x;
    unsigned int depth;

    ShapeLine* shapeLine;
    Shape2* s;
};
