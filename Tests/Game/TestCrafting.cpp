#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Crafting/CraftingService.h"
#include "Game/Source/Crafting/RecipeDatabase.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/Progression/ProgressionState.h"

#include <cassert>

void TestCrafting()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    const rw::game::RecipeDatabase recipes = rw::game::RecipeDatabase::CreateStarterRecipes();
    assert(recipes.FindById("primitive_tool") != nullptr);
    assert(recipes.FindById("camp_bundle") != nullptr);
    assert(recipes.FindById("simple_meal") != nullptr);
    assert(recipes.FindById("workbench_kit") != nullptr);
    assert(recipes.FindById("realm_anchor") != nullptr);
    assert(recipes.FindById("missing_recipe") == nullptr);

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

    rw::game::Inventory lockedAnchorInventory(8);
    assert(lockedAnchorInventory.AddItem(database, "workbench_kit", 1) == 0);
    assert(lockedAnchorInventory.AddItem(database, "fiber", 5) == 0);
    assert(lockedAnchorInventory.AddItem(database, "stone", 5) == 0);
    rw::game::CraftingResult lockedAnchor = rw::game::CraftingService::Craft(
        recipes, database, lockedAnchorInventory, "realm_anchor");
    assert(!lockedAnchor.success);
    assert(lockedAnchor.failureReason == rw::game::CraftingFailureReason::Locked);
    assert(lockedAnchorInventory.TotalQuantity("workbench_kit") == 1);

    rw::game::ProgressionState anchorProgression;
    anchorProgression.SetFlag(rw::game::kRecipeRealmAnchorUnlocked);
    rw::game::Inventory unlockedAnchorInventory(8);
    assert(unlockedAnchorInventory.AddItem(database, "workbench_kit", 1) == 0);
    assert(unlockedAnchorInventory.AddItem(database, "fiber", 5) == 0);
    assert(unlockedAnchorInventory.AddItem(database, "stone", 5) == 0);
    rw::game::CraftingResult unlockedAnchor = rw::game::CraftingService::Craft(
        recipes, database, anchorProgression, unlockedAnchorInventory, "realm_anchor");
    assert(unlockedAnchor.success);
    assert(unlockedAnchorInventory.TotalQuantity("realm_anchor") == 1);
}

