#include "Game/Source/Crafting/CraftingService.h"

#include "Game/Source/Crafting/RecipeDatabase.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Progression/ProgressionState.h"

namespace rw::game {

namespace {

CraftingResult CraftInternal(
    const RecipeDatabase& recipes,
    const ItemDatabase& items,
    const ProgressionState* progression,
    Inventory& inventory,
    const std::string& recipeId);

} // namespace

CraftingResult CraftingService::Craft(
    const RecipeDatabase& recipes,
    const ItemDatabase& items,
    Inventory& inventory,
    const std::string& recipeId)
{
    return CraftInternal(recipes, items, nullptr, inventory, recipeId);
}

CraftingResult CraftingService::Craft(
    const RecipeDatabase& recipes,
    const ItemDatabase& items,
    const ProgressionState& progression,
    Inventory& inventory,
    const std::string& recipeId)
{
    return CraftInternal(recipes, items, &progression, inventory, recipeId);
}

namespace {

CraftingResult CraftInternal(
    const RecipeDatabase& recipes,
    const ItemDatabase& items,
    const ProgressionState* progression,
    Inventory& inventory,
    const std::string& recipeId)
{
    const RecipeDefinition* recipe = recipes.FindById(recipeId);
    if (recipe == nullptr) {
        return {
            false,
            {},
            0,
            CraftingFailureReason::UnknownRecipe,
            "Craft failed: unknown recipe " + recipeId,
            0,
        };
    }

    if (!recipe->unlockId.empty()
        && (progression == nullptr || !progression->HasFlag(recipe->unlockId))) {
        return {
            false,
            {},
            0,
            CraftingFailureReason::Locked,
            "Craft failed: locked recipe " + recipe->id,
            0,
        };
    }

    if (items.FindById(recipe->outputItemId) == nullptr || recipe->outputQuantity <= 0) {
        return {
            false,
            {},
            0,
            CraftingFailureReason::MissingOutputItem,
            "Craft failed: missing output item " + recipe->outputItemId,
            0,
        };
    }

    for (const RecipeIngredient& ingredient : recipe->ingredients) {
        if (!inventory.Contains(ingredient.itemId, ingredient.quantity)) {
            return {
                false,
                {},
                0,
                CraftingFailureReason::MissingIngredient,
                "Craft failed: missing " + ingredient.itemId,
                0,
            };
        }
    }

    Inventory simulated = inventory;
    for (const RecipeIngredient& ingredient : recipe->ingredients) {
        simulated.RemoveItem(ingredient.itemId, ingredient.quantity);
    }

    const int leftover = simulated.AddItem(items, recipe->outputItemId, recipe->outputQuantity);
    if (leftover > 0) {
        return {
            false,
            recipe->outputItemId,
            0,
            CraftingFailureReason::InventoryFull,
            "Craft failed: inventory full for " + recipe->outputItemId,
            leftover,
        };
    }

    for (const RecipeIngredient& ingredient : recipe->ingredients) {
        inventory.RemoveItem(ingredient.itemId, ingredient.quantity);
    }
    inventory.AddItem(items, recipe->outputItemId, recipe->outputQuantity);

    return {
        true,
        recipe->outputItemId,
        recipe->outputQuantity,
        CraftingFailureReason::None,
        "Crafted " + recipe->displayName,
        0,
    };
}

} // namespace

} // namespace rw::game
