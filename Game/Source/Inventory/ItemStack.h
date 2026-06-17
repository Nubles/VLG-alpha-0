#pragma once

#include <string>

namespace rw::game {

struct ItemStack {
    std::string itemId;
    int quantity = 0;

    bool IsEmpty() const
    {
        return itemId.empty() || quantity <= 0;
    }
};

} // namespace rw::game
