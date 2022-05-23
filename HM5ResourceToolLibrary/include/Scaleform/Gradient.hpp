#pragma once

struct RGBA;

struct Gradient
{
    int num;
    unsigned char* ratios;
    RGBA* rgba;
};
