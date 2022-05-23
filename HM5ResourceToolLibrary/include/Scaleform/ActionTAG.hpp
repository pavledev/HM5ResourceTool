#pragma once

struct ActionTAG
{
    unsigned char            op;
    unsigned short           len;
    unsigned char* data;

    ActionTAG* next;
    ActionTAG* prev;

    ActionTAG* parent;
    unsigned char tmp[8]; // store small operands here.
};
