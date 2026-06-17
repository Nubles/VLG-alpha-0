#include "Game/Source/Crafting/RecipeDatabase.h"

#include "Game/Source/Progression/ProgressionFlag.h"

#include <utility>

namespace rw::game {

RecipeDatabase RecipeDatabase::CreateStarterRecipes()
{
    RecipeDatabase database;

    database.AddRecipe({
        "primitive_tool",
        "Primitive Tool",
        "primitive_tool",
        1,
        {
            { "wood", 1 },
            { "stone", 1 },
            { "fiber", 1 },
        },
    });

    database.AddRecipe({
        "camp_bundle",
        "Camp Bundle",
        "camp_bundle",
        1,
        {
            { "wood", 5 },
            { "fiber", 3 },
        },
    });

    database.AddRecipe({
        "simple_meal",
        "Simple Meal",
        "simple_meal",
        1,
        {
            { "raw_food", 2 },
            { "fiber", 1 },
        },
    });

    database.AddRecipe({
        "workbench_kit",
        "Workbench Kit",
        "workbench_kit",
        1,
        {
            { "wood", 10 },
            { "stone", 4 },
        },
    });

    database.AddRecipe({
        "realm_anchor",
        "Realm Anchor",
        "realm_anchor",
        1,
        {
            { "workbench_kit", 1 },
            { "fiber", 5 },
            { "stone", 5 },
        },
        {},
        kRecipeRealmAnchorUnlocked,
    });

    return database;
}

void RecipeDatabase::AddRecipe(RecipeDefinition recipe)
{
    m_recipes.push_back(std::move(recipe));
}

const RecipeDefinition* RecipeDatabase::FindById(const std::string& recipeId) const
{
    for (const RecipeDefinition& recipe : m_recipes) {
        if (recipe.id == recipeId) {
            return &recipe;
        }
    }

    return nullptr;
}

const std::vector<RecipeDefinition>& RecipeDatabase::Recipes() const
{
    return m_recipes;
}

} // namespace rw::game
