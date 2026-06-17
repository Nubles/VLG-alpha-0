#pragma once

#include "Engine/Math/Vec3.h"

namespace rw::math {

struct Transform {
    Vec3 position { 0.0F, 0.0F, 0.0F };
    Vec3 rotationEuler { 0.0F, 0.0F, 0.0F };
    Vec3 scale { 1.0F, 1.0F, 1.0F };
};

} // namespace rw::math

