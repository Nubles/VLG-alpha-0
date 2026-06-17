#pragma once

#include "Engine/Math/Vec3.h"
#include "Game/Source/Inventory/ItemStack.h"

#include <string>
#include <vector>

namespace rw::game {

struct SavedBuildable {
    std::string instanceId;
    std::string buildableId;
    rw::math::Vec3 position;
    float yawRadians = 0.0F;
};

struct SaveData {
    int version = 1;
    std::string biomeId = "mistwood_hollow";
    rw::math::Vec3 playerPosition { 0.0F, 1.8F, 6.0F };
    float playerYawRadians = 3.14159265F;
    float playerPitchRadians = -0.18F;
    float playerHealth = 100.0F;
    float playerStamina = 100.0F;
    std::vector<ItemStack> inventorySlots;
    std::vector<std::string> progressionFlags;
    std::vector<SavedBuildable> placedBuildables;
    std::vector<std::string> depletedGatherableIds;
    bool mistwoodFractureStabilized = false;
    bool mistwoodObjectiveCompleted = false;
    bool realmWispAlive = true;
    rw::math::Vec3 realmWispPosition { 1.2F, 0.7F, -5.0F };
};

} // namespace rw::game
