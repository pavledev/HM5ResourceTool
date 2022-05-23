#pragma once

struct LineStyle;
struct FillStyle;
struct ShapeLine;
struct SRect;

struct Shape2
{
    LineStyle* lineStyles;
    int numberOfLineStyles;
    FillStyle* fillStyles;
    int numberOfFillstyles;
    ShapeLine* lines;
    SRect* bbox; //May be NULL
};
