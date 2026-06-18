#include "Game/Source/Crafting/RecipeDatabase.h"

#include "Engine/Core/Logger.h"
#include "Game/Source/Data/DataTextParser.h"
#include "Game/Source/Progression/ProgressionFlag.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace rw::game {

namespace {

bool HasRequiredRecipeFields(const std::vector<std::string>& fields)
{
    return !fields[0].empty()
        && !fields[1].empty()
        && !fields[2].empty()
        && !fields[3].empty()
        && !fields[6].empty();
}

bool ParseIngredients(
    const std::string& text,
    int lineNumber,
    std::vector<RecipeIngredient>& outIngredients,
    std::string& outError)
{
    const std::vector<std::string> ingredientTexts = data::Split(text, ',');
    if (ingredientTexts.empty()) {
        outError = data::MakeLineError("Recipe", lineNumber, "ingredient list must not be empty");
        return false;
    }

    for (const std::string& ingredientText : ingredientTexts) {
        const std::vector<std::string> parts = data::Split(ingredientText, ':');
        if (parts.size() != 2 || parts[0].empty() || parts[1].empty()) {
            outError = data::MakeLineError("Recipe", lineNumber, "Invalid ingredient '" + ingredientText + "'");
            return false;
        }

        int quantity = 0;
        if (!data::ParsePositiveInt(parts[1], quantity)) {
            outError = data::MakeLineError("Recipe", lineNumber, "ingredient quantity must be greater than 0");
            return false;
        }

        outIngredients.push_back({ parts[0], quantity });
    }

    return true;
}

} // namespace

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

RecipeDatabaseLoadResult RecipeDatabase::LoadFromText(const std::string& text)
{
    RecipeDatabase database;
    std::stringstream stream(text);
    std::string line;
    int lineNumber = 0;

    while (std::getline(stream, line)) {
        ++lineNumber;
        if (data::IsCommentOrBlankLine(line)) {
            continue;
        }

        const std::vector<std::string> fields = data::Split(line, '|');
        if (fields.size() != 7) {
            return {
                false,
                data::MakeLineError("Recipe", lineNumber, "Expected 7 fields"),
                {},
            };
        }

        if (!HasRequiredRecipeFields(fields)) {
            return {
                false,
                data::MakeLineError("Recipe", lineNumber, "Required field is empty"),
                {},
            };
        }

        int outputQuantity = 0;
        if (!data::ParsePositiveInt(fields[3], outputQuantity)) {
            return {
                false,
                data::MakeLineError("Recipe", lineNumber, "output_quantity must be greater than 0"),
                {},
            };
        }

        std::vector<RecipeIngredient> ingredients;
        std::string ingredientError;
        if (!ParseIngredients(fields[6], lineNumber, ingredients, ingredientError)) {
            return { false, ingredientError, {} };
        }

        database.AddRecipe({
            fields[0],
            fields[1],
            fields[2],
            outputQuantity,
            ingredients,
            fields[4],
            fields[5],
        });
    }

    if (database.Recipes().empty()) {
        return { false, "Recipe data did not contain any recipe definitions", {} };
    }

    return { true, "Loaded recipe definitions", database };
}

RecipeDatabaseLoadResult RecipeDatabase::LoadFromFile(const std::string& path)
{
    bool readSuccess = false;
    const std::string text = data::ReadFileText(path, readSuccess);
    if (!readSuccess) {
        return { false, "Could not open recipe data file: " + path, {} };
    }
    return LoadFromText(text);
}

RecipeDatabase RecipeDatabase::CreateFromFileOrFallback(const std::string& path)
{
    const RecipeDatabaseLoadResult loadResult = LoadFromFile(path);
    if (loadResult.success) {
        return loadResult.database;
    }

    rw::core::Logger::Warning(loadResult.message + "; using hardcoded starter recipe definitions.");
    return CreateStarterRecipes();
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
