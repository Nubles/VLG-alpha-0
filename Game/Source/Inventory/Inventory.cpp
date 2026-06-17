#include "Game/Source/Inventory/Inventory.h"

#include <algorithm>
#include <utility>

namespace rw::game {

Inventory::Inventory(int slotCount)
    : m_slots(static_cast<size_t>(std::max(0, slotCount)))
{
}

int Inventory::AddItem(const ItemDatabase& database, const std::string& itemId, int quantity)
{
    if (quantity <= 0) {
        return 0;
    }

    const ItemDefinition* definition = database.FindById(itemId);
    if (definition == nullptr || definition->maxStackSize <= 0) {
        return quantity;
    }

    int remaining = quantity;

    for (ItemStack& slot : m_slots) {
        if (slot.itemId != itemId || slot.quantity >= definition->maxStackSize) {
            continue;
        }

        const int space = definition->maxStackSize - slot.quantity;
        const int moved = std::min(space, remaining);
        slot.quantity += moved;
        remaining -= moved;

        if (remaining == 0) {
            return 0;
        }
    }

    for (ItemStack& slot : m_slots) {
        if (!slot.IsEmpty()) {
            continue;
        }

        const int moved = std::min(definition->maxStackSize, remaining);
        slot.itemId = itemId;
        slot.quantity = moved;
        remaining -= moved;

        if (remaining == 0) {
            return 0;
        }
    }

    return remaining;
}

bool Inventory::Contains(const std::string& itemId, int quantity) const
{
    return TotalQuantity(itemId) >= quantity;
}

bool Inventory::RemoveItem(const std::string& itemId, int quantity)
{
    if (quantity <= 0) {
        return true;
    }

    if (!Contains(itemId, quantity)) {
        return false;
    }

    int remaining = quantity;
    for (ItemStack& slot : m_slots) {
        if (slot.itemId != itemId) {
            continue;
        }

        const int removed = std::min(slot.quantity, remaining);
        slot.quantity -= removed;
        remaining -= removed;

        if (slot.quantity <= 0) {
            slot = {};
        }

        if (remaining == 0) {
            return true;
        }
    }

    return true;
}

void Inventory::ReplaceSlots(std::vector<ItemStack> slots)
{
    m_slots = std::move(slots);
}

const std::vector<ItemStack>& Inventory::Slots() const
{
    return m_slots;
}

int Inventory::SlotCount() const
{
    return static_cast<int>(m_slots.size());
}

int Inventory::TotalQuantity(const std::string& itemId) const
{
    int total = 0;
    for (const ItemStack& slot : m_slots) {
        if (slot.itemId == itemId) {
            total += slot.quantity;
        }
    }

    return total;
}

} // namespace rw::game
