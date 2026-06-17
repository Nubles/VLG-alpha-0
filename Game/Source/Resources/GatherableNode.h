#pragma once

#include "Game/Source/Interactable.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Resources/DropTable.h"
#include "Game/Source/Resources/GatheringResult.h"

#include <string>

namespace rw::game {

class GatherableNode {
public:
    std::string id;
    std::string name;
    rw::math::Transform transform;
    float interactionRadius = 1.0F;
    int remainingGatherCount = 1;
    DropTable dropTable;
    std::string requiredToolId;
    std::string debugMessage;

    bool HitBy(const InteractionRay& ray) const;
    bool IsDepleted() const;
    GatheringResult Gather(const ItemDatabase& database, Inventory& inventory);
};

} // namespace rw::game
