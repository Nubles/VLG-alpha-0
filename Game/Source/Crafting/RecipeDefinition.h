#pragma once

#include <string>
#include <vector>

namespace rw::game {

struct RecipeIngredient {
    std::string itemId;
    int quantity = 0;
};

struct RecipeDefinition {
    std::string id;
    std::string displayName;
    std::string outputItemId;
    int outputQuantity = 1;
    std::vector<RecipeIngredient> ingredients;
    std::string requiredStationId;
    std::string unlockId;
};

} // namespace rw::game
