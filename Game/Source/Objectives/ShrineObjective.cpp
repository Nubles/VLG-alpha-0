#include "Game/Source/Objectives/ShrineObjective.h"

#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/Progression/ProgressionState.h"

namespace rw::game {

namespace {

constexpr const char* kObjectiveId = "mistwood_fracture_shrine";

} // namespace

ObjectiveDefinition ShrineObjective::CreateMistwoodFractureObjective()
{
    return {
        kObjectiveId,
        "Stabilise the Mistwood Fracture",
        {
            { "camp_bundle", 1, true },
            { "primitive_tool", 1, false },
            { "fiber", 3, true },
        },
    };
}

ObjectiveResult ShrineObjective::TryCompleteMistwoodFracture(
    Inventory& inventory,
    ProgressionState& progression,
    ObjectiveState& objectiveState)
{
    const ObjectiveDefinition objective = CreateMistwoodFractureObjective();

    progression.SetFlag(kMistwoodFractureDiscovered);

    if (objectiveState.IsCompleted(objective.id)) {
        return {
            false,
            ObjectiveFailureReason::AlreadyCompleted,
            "The fracture is already stabilised.",
        };
    }

    for (const ObjectiveRequirement& requirement : objective.requirements) {
        if (!inventory.Contains(requirement.itemId, requirement.quantity)) {
            return {
                false,
                ObjectiveFailureReason::MissingRequirement,
                "Missing requirement: " + requirement.itemId,
            };
        }
    }

    for (const ObjectiveRequirement& requirement : objective.requirements) {
        if (requirement.consumeOnCompletion) {
            inventory.RemoveItem(requirement.itemId, requirement.quantity);
        }
    }

    objectiveState.Complete(objective.id);
    progression.SetFlag(kMistwoodShrineCompleted);
    progression.SetFlag(kMistwoodFractureStabilized);
    progression.SetFlag(kRecipeRealmAnchorUnlocked);

    return {
        true,
        ObjectiveFailureReason::None,
        "Mistwood Fracture stabilised. Realm Anchor recipe unlocked.",
    };
}

} // namespace rw::game
