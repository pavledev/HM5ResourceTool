#pragma once

struct Tag;

struct RenderLine
{
    Tag* points; //incremented in 128 byte steps
    int num;
    unsigned int pendingClipdepth;
};
