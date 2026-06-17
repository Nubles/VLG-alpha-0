#include "Engine/Math/Mat4.h"

#include <cmath>

namespace rw::math {

Mat4 Mat4::Identity()
{
    Mat4 result;
    result.m[0] = 1.0F;
    result.m[5] = 1.0F;
    result.m[10] = 1.0F;
    result.m[15] = 1.0F;
    return result;
}

Mat4 Mat4::Perspective(float fovRadians, float aspect, float nearPlane, float farPlane)
{
    Mat4 result;
    const float f = 1.0F / std::tan(fovRadians * 0.5F);

    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
    result.m[11] = -1.0F;
    result.m[14] = (2.0F * farPlane * nearPlane) / (nearPlane - farPlane);

    return result;
}

Mat4 Mat4::LookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
{
    const Vec3 forward = Normalize(target - eye);
    const Vec3 side = Normalize(Cross(forward, up));
    const Vec3 cameraUp = Cross(side, forward);

    Mat4 result = Identity();
    result.m[0] = side.x;
    result.m[4] = cameraUp.x;
    result.m[8] = -forward.x;

    result.m[1] = side.y;
    result.m[5] = cameraUp.y;
    result.m[9] = -forward.y;

    result.m[2] = side.z;
    result.m[6] = cameraUp.z;
    result.m[10] = -forward.z;

    result.m[12] = -Dot(side, eye);
    result.m[13] = -Dot(cameraUp, eye);
    result.m[14] = Dot(forward, eye);
    return result;
}

const float* Mat4::Data() const
{
    return m;
}

} // namespace rw::math
