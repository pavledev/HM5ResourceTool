#pragma once

#include "CharacterType.hpp"

struct Tag;
struct SRect;
struct Shape2;
struct Font;

struct Character
{
    Tag* tag;
    SRect* bbox;
    CharacterType type;

    union
    {
        Shape2* shape;
        Font* font;
    } obj;
};
