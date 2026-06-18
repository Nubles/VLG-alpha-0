#pragma once

#include "Game/Source/Crafting/RecipeDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

struct RecipeDatabaseLoadResult;

class RecipeDatabase {
public:
    static RecipeDatabase CreateStarterRecipes();
    static RecipeDatabaseLoadResult LoadFromText(const std::string& text);
    static RecipeDatabaseLoadResult LoadFromFile(const std::string& path);
    static RecipeDatabase CreateFromFileOrFallback(const std::string& path);

    void AddRecipe(RecipeDefinition recipe);
    const RecipeDefinition* FindById(const std::string& recipeId) const;
    const std::vector<RecipeDefinition>& Recipes() const;

private:
    std::vector<RecipeDefinition> m_recipes;
};

struct RecipeDatabaseLoadResult {
    bool success = false;
    std::string message;
    RecipeDatabase database;
};

} // namespace rw::game
