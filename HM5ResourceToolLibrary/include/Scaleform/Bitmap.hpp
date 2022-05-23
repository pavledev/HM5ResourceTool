#pragma once

struct RGBA;

struct Bitmap
{
    int width;
    int height;
    RGBA* data;
    int id;
    Bitmap* next;
};
