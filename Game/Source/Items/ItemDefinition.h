#pragma once

#include <string>

namespace rw::game {

enum class ItemCategory {
    Resource,
    Food,
    Tool
};

struct ItemDefinition {
    std::string id;
    std::string displayName;
    std::string description;
    ItemCategory category = ItemCategory::Resource;
    int maxStackSize = 1;
    int resourceTier = 0;
};

const char* ToString(ItemCategory category);

} // namespace rw::game
