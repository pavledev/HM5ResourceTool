#pragma once

struct RGBA
{
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;

    RGBA() = default;

    RGBA(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
    {
        this->a = a;
        this->r = r;
        this->g = g;
        this->b = b;
    }
};
