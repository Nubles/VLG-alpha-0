#pragma once

namespace rw::math {

struct Vec3 {
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;

    constexpr Vec3() = default;
    constexpr Vec3(float inX, float inY, float inZ)
        : x(inX)
        , y(inY)
        , z(inZ)
    {
    }

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3& operator+=(const Vec3& other);
};

float Dot(const Vec3& a, const Vec3& b);
Vec3 Cross(const Vec3& a, const Vec3& b);
float Length(const Vec3& value);
Vec3 Normalize(const Vec3& value);

} // namespace rw::math

