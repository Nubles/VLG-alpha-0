#include "Game/Source/Items/ItemDefinition.h"

namespace rw::game {

const char* ToString(ItemCategory category)
{
    switch (category) {
    case ItemCategory::Resource:
        return "Resource";
    case ItemCategory::Food:
        return "Food";
    case ItemCategory::Tool:
        return "Tool";
    }

    return "Unknown";
}

} // namespace rw::game
