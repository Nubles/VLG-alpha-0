#include "Game/Source/Resources/DropTable.h"

#include <algorithm>
#include <utility>

namespace rw::game {

void DropTable::AddDrop(std::string itemId, int minQuantity, int maxQuantity)
{
    m_entries.push_back({
        std::move(itemId),
        std::max(0, minQuantity),
        std::max(0, maxQuantity),
    });
}

std::vector<ItemStack> DropTable::RollDeterministic() const
{
    std::vector<ItemStack> drops;
    for (const DropEntry& entry : m_entries) {
        const int quantity = std::max(entry.minQuantity, entry.maxQuantity);
        if (!entry.itemId.empty() && quantity > 0) {
            drops.push_back({ entry.itemId, quantity });
        }
    }

    return drops;
}

const std::vector<DropEntry>& DropTable::Entries() const
{
    return m_entries;
}

} // namespace rw::game
