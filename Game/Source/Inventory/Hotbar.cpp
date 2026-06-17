#include "Game/Source/Inventory/Hotbar.h"

#include <algorithm>

namespace rw::game {

Hotbar::Hotbar(int size)
    : m_entries(static_cast<size_t>(std::max(0, size)), -1)
{
}

bool Hotbar::AssignSlot(int hotbarIndex, int inventorySlotIndex)
{
    if (hotbarIndex < 0 || hotbarIndex >= static_cast<int>(m_entries.size()) || inventorySlotIndex < 0) {
        return false;
    }

    m_entries[static_cast<size_t>(hotbarIndex)] = inventorySlotIndex;
    return true;
}

bool Hotbar::SelectIndex(int index)
{
    if (index < 0 || index >= static_cast<int>(m_entries.size())) {
        return false;
    }

    m_selectedIndex = index;
    return true;
}

void Hotbar::SelectNext()
{
    if (m_entries.empty()) {
        return;
    }

    m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_entries.size());
}

void Hotbar::SelectPrevious()
{
    if (m_entries.empty()) {
        return;
    }

    m_selectedIndex = (m_selectedIndex + static_cast<int>(m_entries.size()) - 1) % static_cast<int>(m_entries.size());
}

int Hotbar::SelectedIndex() const
{
    return m_selectedIndex;
}

int Hotbar::SelectedInventorySlot() const
{
    if (m_entries.empty()) {
        return -1;
    }

    return m_entries[static_cast<size_t>(m_selectedIndex)];
}

const std::vector<int>& Hotbar::Entries() const
{
    return m_entries;
}

} // namespace rw::game
