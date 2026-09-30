#pragma once

#include "platform/default_types.h"

namespace SOFTDRAW
{

    struct u32_color
    {
        u32_color() = default;

        u32_color(u8 red, u8 green, u8 blue, u8 alpha) : r(red), g(green), b(blue), a(alpha) {}

        u32_color operator*(float s) const {
            return {(u8)(r * s), (u8)(g * s), (u8)(b * s), (u8)(a * s)};
        }

        u32_color operator+(const u32_color& o) const {
            return {(u8)(r + o.r), (u8)(g + o.g), (u8)(b + o.b), (u8)(a + o.a)};
        }

        static u32_color lerp(const u32_color& a, const u32_color& b, float t)
        {
            return a * (1.0f - t) + b * t;
        }

        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

}