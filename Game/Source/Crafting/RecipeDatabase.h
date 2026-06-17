#pragma once

#include "Game/Source/Crafting/RecipeDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

class RecipeDatabase {
public:
    static RecipeDatabase CreateStarterRecipes();

    void AddRecipe(RecipeDefinition recipe);
    const RecipeDefinition* FindById(const std::string& recipeId) const;
    const std::vector<RecipeDefinition>& Recipes() const;

private:
    std::vector<RecipeDefinition> m_recipes;
};

} // namespace rw::game
