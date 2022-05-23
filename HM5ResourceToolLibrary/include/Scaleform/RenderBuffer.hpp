#pragma once

struct RenderBuffer
{
    int width;
    int height;
    int posx, posy;
    void* internal;
};
