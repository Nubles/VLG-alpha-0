#pragma once

#include "Game/Source/Inventory/ItemStack.h"

#include <string>
#include <vector>

namespace rw::game {

struct GatheringResult {
    bool success = false;
    std::string message;
    std::vector<ItemStack> drops;
    int leftoverQuantity = 0;
};

} // namespace rw::game
