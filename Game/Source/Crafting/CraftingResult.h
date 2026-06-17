#pragma once

#include <string>

namespace rw::game {

enum class CraftingFailureReason {
    None,
    UnknownRecipe,
    MissingIngredient,
    MissingOutputItem,
    InventoryFull,
    Locked
};

struct CraftingResult {
    bool success = false;
    std::string craftedItemId;
    int craftedQuantity = 0;
    CraftingFailureReason failureReason = CraftingFailureReason::None;
    std::string message;
    int leftoverOutputQuantity = 0;
};

} // namespace rw::game
