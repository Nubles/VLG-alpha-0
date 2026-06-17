#pragma once

#include "Game/Source/Objectives/ObjectiveDefinition.h"
#include "Game/Source/Objectives/ObjectiveResult.h"
#include "Game/Source/Objectives/ObjectiveState.h"

#include <string>

namespace rw::game {

class Inventory;
class ProgressionState;

class ShrineObjective {
public:
    static ObjectiveDefinition CreateMistwoodFractureObjective();
    static std::string BuildMistwoodFractureRequirementMessage(const Inventory& inventory);

    static ObjectiveResult TryCompleteMistwoodFracture(
        Inventory& inventory,
        ProgressionState& progression,
        ObjectiveState& objectiveState);
};

} // namespace rw::game
