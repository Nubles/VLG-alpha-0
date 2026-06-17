#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Scene/Scene.h"

#include <string>
#include <vector>

namespace rw::game {

struct BuildCost {
    std::string itemId;
    int quantity = 0;
};

struct BuildableDefinition {
    std::string id;
    std::string displayName;
    std::vector<BuildCost> costs;
    rw::scene::DebugPrimitive primitive = rw::scene::DebugPrimitive::Cube;
    rw::math::Vec3 previewScale { 1.0F, 1.0F, 1.0F };
    rw::math::Vec3 placedScale { 1.0F, 1.0F, 1.0F };
    float placementDistance = 3.0F;
    float placementRadius = 0.75F;
    std::string category;
};

} // namespace rw::game
