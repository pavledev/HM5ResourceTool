#pragma once

struct Tag // NEVER access a Tag-Struct directly !
{
    unsigned short id;
    unsigned char* data;
    unsigned int memsize; // to minimize realloc() calls

    unsigned int len; // for Set-Access
    unsigned int pos;  // for Get-Access

    Tag* next;
    Tag* prev;

    unsigned char readBit; // for Bit-Manipulating Functions [read]
    unsigned char writeBit; // [write]
};
