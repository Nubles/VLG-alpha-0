#pragma once

#include "Game/Source/Items/ItemDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

class ItemDatabase {
public:
    static ItemDatabase CreateStarterDatabase();

    void AddDefinition(ItemDefinition definition);
    const ItemDefinition* FindById(const std::string& itemId) const;
    const std::vector<ItemDefinition>& Definitions() const;

private:
    std::vector<ItemDefinition> m_definitions;
};

} // namespace rw::game
