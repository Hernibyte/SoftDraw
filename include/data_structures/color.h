#pragma once

#include "platform/default_types.h"

namespace SOFTDRAW
{

    struct u32_color
    {
        u32_color(u8 red, u8 green, u8 blue, u8 alpha) : r(red), g(green), b(blue), a(alpha) {}

        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

}