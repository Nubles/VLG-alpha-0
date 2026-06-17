#pragma once

#include "Engine/Math/Vec3.h"

namespace rw::math {

struct Mat4 {
    float m[16] = {};

    static Mat4 Identity();
    static Mat4 Perspective(float fovRadians, float aspect, float nearPlane, float farPlane);
    static Mat4 LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

    const float* Data() const;
};

} // namespace rw::math

