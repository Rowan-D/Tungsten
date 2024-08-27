#pragma once
#include "wForgePCH.hpp"
#include <cstring>

void UIny32ToBytesLittleEndian(uint32_t value, uint8_t* buffer)
{
    // Check system endianness
    union
    {
        uint32_t i;
        uint8_t c[4];
    } test = { 0x01020304 };

    if (test.c[0] == 1)
    {
        // Big-endian system
        buffer[0] = value & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
        buffer[2] = (value >> 16) & 0xFF;
        buffer[3] = (value >> 24) & 0xFF;
    }
    else
    {
        // Little-endian system
        std::memcpy(buffer, &value, sizeof(value));
    }
}