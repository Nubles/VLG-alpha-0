#include "Game/Source/Objectives/ShrineObjective.h"

#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/Progression/ProgressionState.h"

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

namespace rw::game {

namespace {

constexpr const char* kObjectiveId = "mistwood_fracture_shrine";

std::string RequirementText(const ObjectiveRequirement& requirement)
{
    std::ostringstream text;
    text << requirement.itemId << " x" << requirement.quantity;
    if (requirement.consumeOnCompletion) {
        text << " consumed";
    } else {
        text << " required, kept";
    }
    return text.str();
}

std::string JoinTexts(const std::vector<std::string>& texts)
{
    std::ostringstream joined;
    for (std::size_t index = 0; index < texts.size(); ++index) {
        if (index > 0) {
            joined << ", ";
        }
        joined << texts[index];
    }
    return joined.str();
}

} // namespace

ObjectiveDefinition ShrineObjective::CreateMistwoodFractureObjective()
{
    return {
        kObjectiveId,
        "Stabilise the Mistwood Fracture",
        {
            { "camp_bundle", 1, true },
            { "fiber", 3, true },
            { "primitive_tool", 1, false },
        },
    };
}

std::string ShrineObjective::BuildMistwoodFractureRequirementMessage(const Inventory& inventory)
{
    const ObjectiveDefinition objective = CreateMistwoodFractureObjective();
    std::vector<std::string> requirementTexts;
    std::vector<std::string> missingTexts;

    for (const ObjectiveRequirement& requirement : objective.requirements) {
        requirementTexts.push_back(RequirementText(requirement));
        const int currentQuantity = inventory.TotalQuantity(requirement.itemId);
        if (currentQuantity < requirement.quantity) {
            std::ostringstream missing;
            missing << requirement.itemId << " x" << (requirement.quantity - currentQuantity);
            missingTexts.push_back(missing.str());
        }
    }

    std::ostringstream message;
    message << "Requirements: " << JoinTexts(requirementTexts);
    if (!missingTexts.empty()) {
        message << ". Missing: " << JoinTexts(missingTexts);
    } else {
        message << ". Ready.";
    }
    return message.str();
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
                BuildMistwoodFractureRequirementMessage(inventory),
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
