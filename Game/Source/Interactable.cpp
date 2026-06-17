#include "Game/Source/Interactable.h"

#include <algorithm>

namespace rw::game {

bool Interactable::HitBy(const InteractionRay& ray) const
{
    const rw::math::Vec3 toCenter = transform.position - ray.origin;
    const float projectedDistance = rw::math::Dot(toCenter, ray.direction);
    if (projectedDistance < 0.0F || projectedDistance > ray.maxDistance) {
        return false;
    }

    const rw::math::Vec3 closestPoint = ray.origin + ray.direction * projectedDistance;
    const float distanceToCenter = rw::math::Length(transform.position - closestPoint);
    return distanceToCenter <= interactionRadius;
}

} // namespace rw::game
