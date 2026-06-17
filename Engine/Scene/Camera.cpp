#include "Engine/Scene/Camera.h"

#include <cmath>

namespace rw::scene {

rw::math::Vec3 Camera::Forward() const
{
    const float cosPitch = std::cos(pitchRadians);
    return rw::math::Normalize({
        std::sin(yawRadians) * cosPitch,
        std::sin(pitchRadians),
        std::cos(yawRadians) * cosPitch,
    });
}

rw::math::Vec3 Camera::Right() const
{
    return rw::math::Normalize(rw::math::Cross(Forward(), { 0.0F, 1.0F, 0.0F }));
}

rw::math::Mat4 Camera::ViewMatrix() const
{
    return rw::math::Mat4::LookAt(position, position + Forward(), { 0.0F, 1.0F, 0.0F });
}

rw::math::Mat4 Camera::ProjectionMatrix(float aspectRatio) const
{
    return rw::math::Mat4::Perspective(verticalFovRadians, aspectRatio, nearPlane, farPlane);
}

} // namespace rw::scene

