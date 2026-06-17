#include "Game/Source/Resources/GatherableNode.h"

#include "Engine/Math/Vec3.h"

namespace rw::game {

bool GatherableNode::HitBy(const InteractionRay& ray) const
{
    const rw::math::Vec3 toCenter = transform.position - ray.origin;
    const float projectedDistance = rw::math::Dot(toCenter, ray.direction);
    if (projectedDistance < 0.0F || projectedDistance > ray.maxDistance) {
        return false;
    }

    const rw::math::Vec3 closestPoint = ray.origin + ray.direction * projectedDistance;
    return rw::math::Length(transform.position - closestPoint) <= interactionRadius;
}

bool GatherableNode::IsDepleted() const
{
    return remainingGatherCount <= 0;
}

GatheringResult GatherableNode::Gather(const ItemDatabase& database, Inventory& inventory)
{
    if (IsDepleted()) {
        return { false, name + " is depleted.", {}, 0 };
    }

    if (!requiredToolId.empty() && !inventory.Contains(requiredToolId, 1)) {
        return { false, name + " requires " + requiredToolId + ".", {}, 0 };
    }

    GatheringResult result;
    result.success = true;
    result.message = debugMessage.empty() ? "Gathered " + name + "." : debugMessage;

    for (const ItemStack& drop : dropTable.RollDeterministic()) {
        const int leftover = inventory.AddItem(database, drop.itemId, drop.quantity);
        const int added = drop.quantity - leftover;
        if (added > 0) {
            result.drops.push_back({ drop.itemId, added });
        }
        result.leftoverQuantity += leftover;
    }

    --remainingGatherCount;
    if (result.leftoverQuantity > 0) {
        result.message += " Inventory full; leftover items were not added.";
    }

    return result;
}

} // namespace rw::game
