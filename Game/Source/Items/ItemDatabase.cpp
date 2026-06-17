#include "Game/Source/Items/ItemDatabase.h"

#include <utility>

namespace rw::game {

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
    return database;
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
