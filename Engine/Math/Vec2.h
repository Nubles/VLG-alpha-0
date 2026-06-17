#pragma once

namespace rw::math {

struct Vec2 {
    float x = 0.0F;
    float y = 0.0F;

    constexpr Vec2() = default;
    constexpr Vec2(float inX, float inY)
        : x(inX)
        , y(inY)
    {
    }
};

} // namespace rw::math

