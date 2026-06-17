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
