#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Objectives/ShrineObjective.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/Progression/ProgressionState.h"

#include <cassert>
#include <string>

void TestProgressionObjectives()
{
    rw::game::ProgressionState progression;
    assert(!progression.HasFlag("unknown_flag"));
    progression.SetFlag(rw::game::kMistwoodFractureDiscovered);
    assert(progression.HasFlag(rw::game::kMistwoodFractureDiscovered));
    progression.Clear();
    assert(!progression.HasFlag(rw::game::kMistwoodFractureDiscovered));

    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    rw::game::ProgressionState objectiveProgression;
    rw::game::ObjectiveState objectiveState;
    rw::game::Inventory objectiveMissingInventory(8);
    assert(objectiveMissingInventory.AddItem(database, "camp_bundle", 1) == 0);
    assert(objectiveMissingInventory.AddItem(database, "fiber", 1) == 0);
    rw::game::ObjectiveResult objectiveMissing = rw::game::ShrineObjective::TryCompleteMistwoodFracture(
        objectiveMissingInventory, objectiveProgression, objectiveState);
    assert(!objectiveMissing.success);
    assert(objectiveMissing.failureReason == rw::game::ObjectiveFailureReason::MissingRequirement);
    assert(objectiveMissingInventory.TotalQuantity("camp_bundle") == 1);
    assert(objectiveProgression.HasFlag(rw::game::kMistwoodFractureDiscovered));
    assert(!objectiveProgression.HasFlag(rw::game::kMistwoodShrineCompleted));
    assert(objectiveMissing.message.find("Requirements: camp_bundle x1 consumed") != std::string::npos);
    assert(objectiveMissing.message.find("fiber x3 consumed") != std::string::npos);
    assert(objectiveMissing.message.find("primitive_tool x1 required, kept") != std::string::npos);
    assert(objectiveMissing.message.find("Missing: fiber x2, primitive_tool x1") != std::string::npos);

    const std::string readyMessage = rw::game::ShrineObjective::BuildMistwoodFractureRequirementMessage(
        objectiveMissingInventory);
    assert(readyMessage.find("primitive_tool x1 required, kept") != std::string::npos);

    rw::game::ProgressionState completeProgression;
    rw::game::ObjectiveState completeObjectiveState;
    rw::game::Inventory objectiveInventory(8);
    assert(objectiveInventory.AddItem(database, "camp_bundle", 1) == 0);
    assert(objectiveInventory.AddItem(database, "primitive_tool", 1) == 0);
    assert(objectiveInventory.AddItem(database, "fiber", 3) == 0);
    rw::game::ObjectiveResult objectiveComplete = rw::game::ShrineObjective::TryCompleteMistwoodFracture(
        objectiveInventory, completeProgression, completeObjectiveState);
    assert(objectiveComplete.success);
    assert(objectiveInventory.TotalQuantity("camp_bundle") == 0);
    assert(objectiveInventory.TotalQuantity("fiber") == 0);
    assert(objectiveInventory.TotalQuantity("primitive_tool") == 1);
    assert(completeProgression.HasFlag(rw::game::kMistwoodShrineCompleted));
    assert(completeProgression.HasFlag(rw::game::kMistwoodFractureStabilized));
    assert(completeProgression.HasFlag(rw::game::kRecipeRealmAnchorUnlocked));
}

