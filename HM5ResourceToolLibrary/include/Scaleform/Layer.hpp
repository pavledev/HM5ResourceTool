#pragma once

struct RenderPoint;

struct Layer
{
    int fillID;
    RenderPoint* p;
    Layer* next;
    Layer* prev;
};
