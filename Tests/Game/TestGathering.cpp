#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Biomes/BiomeLayoutBuilder.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Resources/DropTable.h"
#include "Game/Source/Resources/GatherableNode.h"
#include "Game/Source/Runtime/SandboxWorldState.h"

#include <cassert>
#include <string>
#include <vector>

void TestGathering()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();

    rw::game::DropTable dropTable;
    dropTable.AddDrop("wood", 4, 4);
    const std::vector<rw::game::ItemStack> drops = dropTable.RollDeterministic();
    assert(drops.size() == 1);
    assert(drops[0].itemId == "wood");
    assert(drops[0].quantity == 4);

    rw::game::Inventory gatheringInventory(4);
    rw::game::GatherableNode branchPile;
    branchPile.name = "Branch Pile";
    branchPile.remainingGatherCount = 1;
    branchPile.dropTable.AddDrop("wood", 3, 3);
    rw::game::GatheringResult branchResult = branchPile.Gather(database, gatheringInventory);
    assert(branchResult.success);
    assert(gatheringInventory.TotalQuantity("wood") == 3);
    assert(branchPile.IsDepleted());

    rw::game::GatheringResult depletedResult = branchPile.Gather(database, gatheringInventory);
    assert(!depletedResult.success);
    assert(gatheringInventory.TotalQuantity("wood") == 3);

    rw::game::Inventory toolCheckInventory(4);
    rw::game::GatherableNode crackedRock;
    crackedRock.name = "Cracked Rock";
    crackedRock.remainingGatherCount = 1;
    crackedRock.requiredToolId = "primitive_tool";
    crackedRock.dropTable.AddDrop("stone", 8, 8);
    rw::game::GatheringResult missingTool = crackedRock.Gather(database, toolCheckInventory);
    assert(!missingTool.success);
    assert(toolCheckInventory.TotalQuantity("stone") == 0);
    assert(!crackedRock.IsDepleted());

    assert(toolCheckInventory.AddItem(database, "primitive_tool", 1) == 0);
    rw::game::GatheringResult withTool = crackedRock.Gather(database, toolCheckInventory);
    assert(withTool.success);
    assert(toolCheckInventory.TotalQuantity("stone") == 8);
    assert(crackedRock.IsDepleted());

    rw::game::Inventory overflowInventory(1);
    rw::game::GatherableNode largePile;
    largePile.name = "Large Branch Pile";
    largePile.remainingGatherCount = 1;
    largePile.dropTable.AddDrop("wood", 60, 60);
    rw::game::GatheringResult overflow = largePile.Gather(database, overflowInventory);
    assert(overflow.success);
    assert(overflowInventory.TotalQuantity("wood") == 50);
    assert(overflow.leftoverQuantity == 10);

    const rw::game::BiomeLayout mistwoodLayout = rw::game::BiomeLayoutBuilder::BuildMistwoodHollow();
    rw::game::SandboxWorldState worldState;
    std::vector<rw::game::GatherableNode> gatherableState = mistwoodLayout.gatherableNodes;
    assert(!gatherableState.empty());
    gatherableState[0].remainingGatherCount = 0;
    const std::vector<std::string> depletedIds = worldState.DepletedGatherableIds(gatherableState);
    assert(depletedIds.size() == 1);
    assert(depletedIds[0] == "branch_pile_01");

    for (rw::game::GatherableNode& node : gatherableState) {
        node.remainingGatherCount = 1;
    }
    const int restoredDepletionCount = worldState.ApplyDepletedGatherableIds(
        gatherableState, { "branch_pile_01", "unknown_node" });
    assert(restoredDepletionCount == 1);
    assert(gatherableState[0].IsDepleted());
}

