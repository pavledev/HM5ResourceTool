#pragma once

struct RenderLine;
struct Bitmap;
struct RGBA;

struct RenderBufferInternal
{
    RenderLine* lines;
    Bitmap* bitmaps;
    int antialize;
    int multiply;
    int width2, height2;
    int shapes;
    int ymin, ymax;

    RGBA* img;
    int* zbuf;
};
