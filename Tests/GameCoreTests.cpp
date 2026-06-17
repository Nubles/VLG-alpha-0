#include "Game/Source/Crafting/CraftingService.h"
#include "Game/Source/Crafting/RecipeDatabase.h"
#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Resources/GatherableNode.h"

#include <cassert>

int main()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();

    const rw::game::ItemDefinition* wood = database.FindById("wood");
    assert(wood != nullptr);
    assert(wood->displayName == "Wood");
    assert(wood->maxStackSize == 50);
    assert(database.FindById("missing_item") == nullptr);
    assert(database.FindById("camp_bundle") != nullptr);
    assert(database.FindById("simple_meal") != nullptr);
    assert(database.FindById("workbench_kit") != nullptr);

    const rw::game::RecipeDatabase recipes = rw::game::RecipeDatabase::CreateStarterRecipes();
    assert(recipes.FindById("primitive_tool") != nullptr);
    assert(recipes.FindById("camp_bundle") != nullptr);
    assert(recipes.FindById("simple_meal") != nullptr);
    assert(recipes.FindById("workbench_kit") != nullptr);
    assert(recipes.FindById("missing_recipe") == nullptr);

    rw::game::Inventory inventory(2);
    assert(inventory.SlotCount() == 2);
    assert(inventory.AddItem(database, "wood", 10) == 0);
    assert(inventory.Slots()[0].itemId == "wood");
    assert(inventory.Slots()[0].quantity == 10);

    assert(inventory.AddItem(database, "wood", 35) == 0);
    assert(inventory.Slots()[0].quantity == 45);

    assert(inventory.AddItem(database, "wood", 10) == 0);
    assert(inventory.Slots()[0].quantity == 50);
    assert(inventory.Slots()[1].itemId == "wood");
    assert(inventory.Slots()[1].quantity == 5);

    assert(inventory.AddItem(database, "wood", 60) == 15);
    assert(inventory.Slots()[0].quantity == 50);
    assert(inventory.Slots()[1].quantity == 50);

    assert(inventory.Contains("wood", 100));
    assert(!inventory.Contains("wood", 101));
    assert(inventory.RemoveItem("wood", 30));
    assert(inventory.TotalQuantity("wood") == 70);
    assert(!inventory.RemoveItem("wood", 100));
    assert(inventory.TotalQuantity("wood") == 70);
    assert(inventory.AddItem(database, "unknown", 4) == 4);

    rw::game::Inventory toolInventory(2);
    assert(toolInventory.AddItem(database, "primitive_tool", 2) == 0);
    assert(toolInventory.Slots()[0].quantity == 1);
    assert(toolInventory.Slots()[1].quantity == 1);

    rw::game::Hotbar hotbar(8);
    assert(hotbar.AssignSlot(0, 0));
    assert(hotbar.AssignSlot(1, 1));
    assert(!hotbar.AssignSlot(8, 0));
    assert(hotbar.SelectedIndex() == 0);
    assert(hotbar.SelectedInventorySlot() == 0);
    hotbar.SelectNext();
    assert(hotbar.SelectedIndex() == 1);
    assert(hotbar.SelectedInventorySlot() == 1);
    hotbar.SelectPrevious();
    assert(hotbar.SelectedIndex() == 0);
    assert(hotbar.SelectIndex(7));
    assert(hotbar.SelectedIndex() == 7);
    assert(!hotbar.SelectIndex(8));

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

    rw::game::Inventory unknownRecipeInventory(4);
    rw::game::CraftingResult unknownRecipe = rw::game::CraftingService::Craft(
        recipes, database, unknownRecipeInventory, "missing_recipe");
    assert(!unknownRecipe.success);
    assert(unknownRecipe.failureReason == rw::game::CraftingFailureReason::UnknownRecipe);

    rw::game::Inventory missingIngredientInventory(4);
    assert(missingIngredientInventory.AddItem(database, "wood", 1) == 0);
    rw::game::CraftingResult missingIngredient = rw::game::CraftingService::Craft(
        recipes, database, missingIngredientInventory, "primitive_tool");
    assert(!missingIngredient.success);
    assert(missingIngredient.failureReason == rw::game::CraftingFailureReason::MissingIngredient);
    assert(missingIngredientInventory.TotalQuantity("wood") == 1);
    assert(missingIngredientInventory.TotalQuantity("primitive_tool") == 0);

    rw::game::Inventory craftingInventory(4);
    assert(craftingInventory.AddItem(database, "wood", 1) == 0);
    assert(craftingInventory.AddItem(database, "stone", 1) == 0);
    assert(craftingInventory.AddItem(database, "fiber", 1) == 0);
    rw::game::CraftingResult primitiveTool = rw::game::CraftingService::Craft(
        recipes, database, craftingInventory, "primitive_tool");
    assert(primitiveTool.success);
    assert(primitiveTool.craftedItemId == "primitive_tool");
    assert(primitiveTool.craftedQuantity == 1);
    assert(craftingInventory.TotalQuantity("wood") == 0);
    assert(craftingInventory.TotalQuantity("stone") == 0);
    assert(craftingInventory.TotalQuantity("fiber") == 0);
    assert(craftingInventory.TotalQuantity("primitive_tool") == 1);

    rw::game::Inventory fullInventory(1);
    assert(fullInventory.AddItem(database, "wood", 10) == 0);
    rw::game::CraftingResult fullResult = rw::game::CraftingService::Craft(
        recipes, database, fullInventory, "camp_bundle");
    assert(!fullResult.success);
    assert(fullResult.failureReason == rw::game::CraftingFailureReason::MissingIngredient);
    assert(fullInventory.TotalQuantity("wood") == 10);
    assert(fullInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::Inventory outputBlockedInventory(2);
    assert(outputBlockedInventory.AddItem(database, "wood", 50) == 0);
    assert(outputBlockedInventory.AddItem(database, "fiber", 50) == 0);
    rw::game::CraftingResult outputBlocked = rw::game::CraftingService::Craft(
        recipes, database, outputBlockedInventory, "camp_bundle");
    assert(!outputBlocked.success);
    assert(outputBlocked.failureReason == rw::game::CraftingFailureReason::InventoryFull);
    assert(outputBlocked.leftoverOutputQuantity == 1);
    assert(outputBlockedInventory.TotalQuantity("wood") == 50);
    assert(outputBlockedInventory.TotalQuantity("fiber") == 50);
    assert(outputBlockedInventory.TotalQuantity("camp_bundle") == 0);

    return 0;
}
