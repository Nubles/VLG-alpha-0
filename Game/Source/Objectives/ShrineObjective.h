#pragma once

#include "Game/Source/Objectives/ObjectiveDefinition.h"
#include "Game/Source/Objectives/ObjectiveResult.h"
#include "Game/Source/Objectives/ObjectiveState.h"

namespace rw::game {

class Inventory;
class ProgressionState;

class ShrineObjective {
public:
    static ObjectiveDefinition CreateMistwoodFractureObjective();

    static ObjectiveResult TryCompleteMistwoodFracture(
        Inventory& inventory,
        ProgressionState& progression,
        ObjectiveState& objectiveState);
};

} // namespace rw::game
