#include "Tests/Game/GameTestSuite.h"

#include "Engine/Scene/Camera.h"
#include "Game/Source/Building/BuildableDatabase.h"
#include "Game/Source/Building/BuildPlacementController.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Runtime/SandboxWorldState.h"

#include <cassert>
#include <vector>

void TestBuilding()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    const rw::game::BuildableDatabase buildables = rw::game::BuildableDatabase::CreateStarterBuildables();
    assert(buildables.FindById("camp_marker") != nullptr);
    assert(buildables.FindById("workbench_stub") != nullptr);
    assert(buildables.FindById("simple_wall") != nullptr);
    assert(buildables.FindById("missing_buildable") == nullptr);

    rw::game::SandboxWorldState worldState;
    rw::game::PlacedBuildable firstPlaced;
    firstPlaced.buildableId = "camp_marker";
    firstPlaced.displayName = "Camp Marker";
    worldState.RegisterPlacedBuildable(firstPlaced);
    assert(worldState.PlacedBuildables()[0].instanceId == "placed_0001");

    rw::game::PlacedBuildable loadedPlaced;
    loadedPlaced.instanceId = "placed_0007";
    loadedPlaced.buildableId = "camp_marker";
    loadedPlaced.displayName = "Camp Marker";
    worldState.ReplacePlacedBuildables({ loadedPlaced });
    assert(worldState.AllocatePlacedBuildableId() == "placed_0008");

    rw::scene::Camera buildCamera;
    buildCamera.position = { 0.0F, 1.6F, 0.0F };
    buildCamera.yawRadians = 0.0F;
    buildCamera.pitchRadians = 0.0F;

    rw::game::BuildPlacementController missingCostBuild;
    missingCostBuild.SelectBuildable("camp_marker");
    missingCostBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory missingBuildInventory(4);
    const std::vector<rw::game::PlacedBuildable> noPlacedBuildables;
    rw::game::BuildPlacementResult missingBuildCost = missingCostBuild.TryPlace(
        buildables, database, missingBuildInventory, noPlacedBuildables);
    assert(!missingBuildCost.success);
    assert(missingBuildCost.failureReason == rw::game::BuildPlacementFailureReason::MissingCost);
    assert(missingBuildInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::BuildPlacementController campBuild;
    campBuild.SelectBuildable("camp_marker");
    campBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory campBuildInventory(4);
    assert(campBuildInventory.AddItem(database, "camp_bundle", 1) == 0);
    rw::game::BuildPlacementResult campPlaced = campBuild.TryPlace(
        buildables, database, campBuildInventory, noPlacedBuildables);
    assert(campPlaced.success);
    assert(campPlaced.placed.buildableId == "camp_marker");
    assert(campBuildInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::BuildPlacementController workbenchBuild;
    workbenchBuild.SelectBuildable("workbench_stub");
    workbenchBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory workbenchInventory(4);
    assert(workbenchInventory.AddItem(database, "workbench_kit", 1) == 0);
    rw::game::BuildPlacementResult workbenchPlaced = workbenchBuild.TryPlace(
        buildables, database, workbenchInventory, noPlacedBuildables);
    assert(workbenchPlaced.success);
    assert(workbenchInventory.TotalQuantity("workbench_kit") == 0);

    rw::game::BuildPlacementController wallBuild;
    wallBuild.SelectBuildable("simple_wall");
    wallBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory wallInventory(4);
    assert(wallInventory.AddItem(database, "wood", 3) == 0);
    assert(wallInventory.AddItem(database, "fiber", 1) == 0);
    rw::game::BuildPlacementResult wallPlaced = wallBuild.TryPlace(
        buildables, database, wallInventory, noPlacedBuildables);
    assert(wallPlaced.success);
    assert(wallInventory.TotalQuantity("wood") == 0);
    assert(wallInventory.TotalQuantity("fiber") == 0);

    rw::game::BuildableDatabase invalidBuildables;
    invalidBuildables.AddBuildable({
        "too_close",
        "Too Close",
        { { "wood", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 1.0F, 1.0F, 1.0F },
        { 1.0F, 1.0F, 1.0F },
        0.5F,
        0.75F,
        "Tests",
    });
    rw::game::BuildPlacementController invalidBuild;
    invalidBuild.SelectBuildable("too_close");
    invalidBuild.UpdatePreview(invalidBuildables, buildCamera);
    rw::game::Inventory invalidBuildInventory(4);
    assert(invalidBuildInventory.AddItem(database, "wood", 1) == 0);
    rw::game::BuildPlacementResult invalidPlacement = invalidBuild.TryPlace(
        invalidBuildables, database, invalidBuildInventory, noPlacedBuildables);
    assert(!invalidPlacement.success);
    assert(invalidPlacement.failureReason == rw::game::BuildPlacementFailureReason::InvalidPlacement);
    assert(invalidBuildInventory.TotalQuantity("wood") == 1);

    rw::game::BuildPlacementController overlapBuild;
    overlapBuild.SelectBuildable("camp_marker");
    overlapBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory overlapInventory(4);
    assert(overlapInventory.AddItem(database, "camp_bundle", 1) == 0);
    const std::vector<rw::game::PlacedBuildable> placedForOverlap { campPlaced.placed };
    rw::game::BuildPlacementResult overlapResult = overlapBuild.TryPlace(
        buildables, database, overlapInventory, placedForOverlap);
    assert(!overlapResult.success);
    assert(overlapResult.failureReason == rw::game::BuildPlacementFailureReason::Overlap);
    assert(overlapInventory.TotalQuantity("camp_bundle") == 1);
}

