#pragma once

#include "Game/Source/Crafting/CraftingResult.h"

#include <string>

namespace rw::game {

class Inventory;
class ItemDatabase;
class RecipeDatabase;

class CraftingService {
public:
    static CraftingResult Craft(
        const RecipeDatabase& recipes,
        const ItemDatabase& items,
        Inventory& inventory,
        const std::string& recipeId);
};

} // namespace rw::game
