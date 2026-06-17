#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Math/Vec3.h"

#include <string>

namespace rw::game {

struct InteractionRay {
    rw::math::Vec3 origin;
    rw::math::Vec3 direction;
    float maxDistance = 5.0F;
};

class Interactable {
public:
    std::string name;
    rw::math::Transform transform;
    float interactionRadius = 1.0F;
    std::string debugMessage;

    bool HitBy(const InteractionRay& ray) const;
};

} // namespace rw::game
