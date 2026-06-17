#pragma once

#include <vector>

namespace rw::game {

class Hotbar {
public:
    explicit Hotbar(int size);

    bool AssignSlot(int hotbarIndex, int inventorySlotIndex);
    bool SelectIndex(int index);
    void SelectNext();
    void SelectPrevious();

    int SelectedIndex() const;
    int SelectedInventorySlot() const;
    const std::vector<int>& Entries() const;

private:
    std::vector<int> m_entries;
    int m_selectedIndex = 0;
};

} // namespace rw::game
