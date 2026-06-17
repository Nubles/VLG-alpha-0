#pragma once

#include "Engine/Math/Mat4.h"
#include "Engine/Math/Vec3.h"

namespace rw::scene {

class Camera {
public:
    rw::math::Vec3 position { 0.0F, 2.0F, 6.0F };
    float yawRadians = 3.14159265F;
    float pitchRadians = -0.25F;
    float verticalFovRadians = 1.04719755F;
    float nearPlane = 0.1F;
    float farPlane = 250.0F;

    rw::math::Vec3 Forward() const;
    rw::math::Vec3 Right() const;
    rw::math::Mat4 ViewMatrix() const;
    rw::math::Mat4 ProjectionMatrix(float aspectRatio) const;
};

} // namespace rw::scene

