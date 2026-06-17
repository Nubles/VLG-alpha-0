#pragma once

#include "Game/Source/Inventory/ItemStack.h"

#include <string>
#include <vector>

namespace rw::game {

struct DropEntry {
    std::string itemId;
    int minQuantity = 0;
    int maxQuantity = 0;
};

class DropTable {
public:
    void AddDrop(std::string itemId, int minQuantity, int maxQuantity);
    std::vector<ItemStack> RollDeterministic() const;
    const std::vector<DropEntry>& Entries() const;

private:
    std::vector<DropEntry> m_entries;
};

} // namespace rw::game
