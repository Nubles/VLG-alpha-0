#pragma once

#include "Game/Source/Inventory/ItemStack.h"
#include "Game/Source/Items/ItemDatabase.h"

#include <string>
#include <vector>

namespace rw::game {

class Inventory {
public:
    explicit Inventory(int slotCount);

    int AddItem(const ItemDatabase& database, const std::string& itemId, int quantity);
    bool Contains(const std::string& itemId, int quantity = 1) const;
    bool RemoveItem(const std::string& itemId, int quantity);
    void ReplaceSlots(std::vector<ItemStack> slots);

    const std::vector<ItemStack>& Slots() const;
    int SlotCount() const;
    int TotalQuantity(const std::string& itemId) const;

private:
    std::vector<ItemStack> m_slots;
};

} // namespace rw::game
