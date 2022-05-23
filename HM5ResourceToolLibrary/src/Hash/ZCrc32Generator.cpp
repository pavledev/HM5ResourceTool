#include "pch.h"
#include "Hash/ZCrc32Generator.hpp"

unsigned int ZCrc32Generator::Crc32(const char* pData)
{
    unsigned int nDataSize = strlen(pData);
    unsigned int crc = 0xFFFFFFFF;

    for (unsigned int i = 0; i < nDataSize; ++i)
    {
        crc = crc32Table[*pData ^ (crc & 0xFF)] ^ (crc >> 8);
        pData++;
    }

    return crc ^ 0xFFFFFFFF;
}

unsigned int ZCrc32Generator::Crc32(const char* pData, unsigned int nDataSize)
{
    unsigned int crc = 0xFFFFFFFF;

    for (unsigned int i = 0; i < nDataSize; ++i)
    {
        crc = crc32Table[*pData ^ (crc & 0xFF)] ^ (crc >> 8);
        pData++;
    }

    return crc ^ 0xFFFFFFFF;
}
