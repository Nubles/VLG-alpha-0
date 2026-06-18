#pragma once

#include "Game/Source/Items/ItemDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

struct ItemDatabaseLoadResult;

class ItemDatabase {
public:
    static ItemDatabase CreateStarterDatabase();
    static ItemDatabaseLoadResult LoadFromText(const std::string& text);
    static ItemDatabaseLoadResult LoadFromFile(const std::string& path);
    static ItemDatabase CreateFromFileOrFallback(const std::string& path);

    void AddDefinition(ItemDefinition definition);
    const ItemDefinition* FindById(const std::string& itemId) const;
    const std::vector<ItemDefinition>& Definitions() const;

private:
    std::vector<ItemDefinition> m_definitions;
};

struct ItemDatabaseLoadResult {
    bool success = false;
    std::string message;
    ItemDatabase database;
};

} // namespace rw::game
