#include "Game/Source/Items/ItemDatabase.h"

#include "Engine/Core/Logger.h"
#include "Game/Source/Data/DataTextParser.h"

#include <sstream>
#include <string>
#include <utility>

namespace rw::game {

namespace {

bool ParseCategory(const std::string& text, ItemCategory& outCategory)
{
    if (text == "Resource") {
        outCategory = ItemCategory::Resource;
        return true;
    }
    if (text == "Food") {
        outCategory = ItemCategory::Food;
        return true;
    }
    if (text == "Tool") {
        outCategory = ItemCategory::Tool;
        return true;
    }
    return false;
}

} // namespace

ItemDatabase ItemDatabase::CreateStarterDatabase()
{
    ItemDatabase database;
    database.AddDefinition({ "wood", "Wood", "A sturdy piece of weathered timber.", ItemCategory::Resource, 50, 0 });
    database.AddDefinition({ "stone", "Stone", "A useful chunk of rough stone.", ItemCategory::Resource, 50, 0 });
    database.AddDefinition({ "fiber", "Fiber", "Tough plant fiber for simple bindings.", ItemCategory::Resource, 50, 0 });
    database.AddDefinition({ "raw_food", "Raw Food", "Unprepared forage from the wilds.", ItemCategory::Food, 20, 0 });
    database.AddDefinition({ "primitive_tool", "Primitive Tool", "A crude tool for early survival work.", ItemCategory::Tool, 1, 0 });
    database.AddDefinition({ "camp_bundle", "Camp Bundle", "A packed camp starter for later building tests.", ItemCategory::Tool, 10, 0 });
    database.AddDefinition({ "simple_meal", "Simple Meal", "Basic prepared food for future survival tests.", ItemCategory::Food, 20, 0 });
    database.AddDefinition({ "workbench_kit", "Workbench Kit", "A packed workbench kit for later station tests.", ItemCategory::Tool, 1, 0 });
    database.AddDefinition({ "realm_anchor", "Realm Anchor", "A crude anchor of stabilized realm energy. It has no world function yet.", ItemCategory::Tool, 1, 0 });
    return database;
}

ItemDatabaseLoadResult ItemDatabase::LoadFromText(const std::string& text)
{
    ItemDatabase database;
    std::stringstream stream(text);
    std::string line;
    int lineNumber = 0;

    while (std::getline(stream, line)) {
        ++lineNumber;
        if (data::IsCommentOrBlankLine(line)) {
            continue;
        }

        const std::vector<std::string> fields = data::Split(line, '|');
        if (fields.size() != 6) {
            return {
                false,
                data::MakeLineError("Item", lineNumber, "Expected 6 fields"),
                {},
            };
        }

        for (const std::string& field : fields) {
            if (field.empty()) {
                return {
                    false,
                    data::MakeLineError("Item", lineNumber, "Required field is empty"),
                    {},
                };
            }
        }

        ItemCategory category = ItemCategory::Resource;
        if (!ParseCategory(fields[3], category)) {
            return {
                false,
                data::MakeLineError("Item", lineNumber, "Unknown item category '" + fields[3] + "'"),
                {},
            };
        }

        int maxStackSize = 0;
        int resourceTier = 0;
        if (!data::ParsePositiveInt(fields[4], maxStackSize)) {
            return {
                false,
                data::MakeLineError("Item", lineNumber, "max_stack must be greater than 0"),
                {},
            };
        }
        try {
            resourceTier = std::stoi(fields[5]);
        } catch (...) {
            return {
                false,
                data::MakeLineError("Item", lineNumber, "Invalid numeric field"),
                {},
            };
        }

        database.AddDefinition({
            fields[0],
            fields[1],
            fields[2],
            category,
            maxStackSize,
            resourceTier,
        });
    }

    if (database.Definitions().empty()) {
        return { false, "Item data did not contain any item definitions", {} };
    }

    return { true, "Loaded item definitions", database };
}

ItemDatabaseLoadResult ItemDatabase::LoadFromFile(const std::string& path)
{
    bool readSuccess = false;
    const std::string text = data::ReadFileText(path, readSuccess);
    if (!readSuccess) {
        return { false, "Could not open item data file: " + path, {} };
    }
    return LoadFromText(text);
}

ItemDatabase ItemDatabase::CreateFromFileOrFallback(const std::string& path)
{
    const ItemDatabaseLoadResult loadResult = LoadFromFile(path);
    if (loadResult.success) {
        return loadResult.database;
    }

    rw::core::Logger::Warning(loadResult.message + "; using hardcoded starter item definitions.");
    return CreateStarterDatabase();
}

void ItemDatabase::AddDefinition(ItemDefinition definition)
{
    m_definitions.push_back(std::move(definition));
}

const ItemDefinition* ItemDatabase::FindById(const std::string& itemId) const
{
    for (const ItemDefinition& definition : m_definitions) {
        if (definition.id == itemId) {
            return &definition;
        }
    }

    return nullptr;
}

const std::vector<ItemDefinition>& ItemDatabase::Definitions() const
{
    return m_definitions;
}

} // namespace rw::game
