#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Scene/Scene.h"

#include <string>

namespace rw::game {

enum class BuildPlacementFailureReason {
    None,
    BuildModeInactive,
    NoBuildableSelected,
    UnknownBuildable,
    MissingCost,
    InvalidPlacement,
    Overlap
};

struct PlacedBuildable {
    std::string buildableId;
    std::string displayName;
    rw::math::Transform transform;
    rw::scene::DebugPrimitive primitive = rw::scene::DebugPrimitive::Cube;
    float placementRadius = 0.75F;
};

struct BuildPlacementResult {
    bool success = false;
    BuildPlacementFailureReason failureReason = BuildPlacementFailureReason::None;
    std::string message;
    PlacedBuildable placed;
};

} // namespace rw::game
