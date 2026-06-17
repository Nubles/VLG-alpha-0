#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"

#include <cassert>

void TestInventoryAndHotbar()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    const rw::game::ItemDefinition* wood = database.FindById("wood");
    assert(wood != nullptr);
    assert(wood->displayName == "Wood");
    assert(wood->maxStackSize == 50);
    assert(database.FindById("missing_item") == nullptr);
    assert(database.FindById("camp_bundle") != nullptr);
    assert(database.FindById("simple_meal") != nullptr);
    assert(database.FindById("workbench_kit") != nullptr);
    assert(database.FindById("realm_anchor") != nullptr);

    rw::game::Inventory inventory(2);
    assert(inventory.SlotCount() == 2);
    assert(inventory.AddItem(database, "wood", 10) == 0);
    assert(inventory.Slots()[0].itemId == "wood");
    assert(inventory.Slots()[0].quantity == 10);
    assert(inventory.AddItem(database, "wood", 35) == 0);
    assert(inventory.Slots()[0].quantity == 45);
    assert(inventory.AddItem(database, "wood", 10) == 0);
    assert(inventory.Slots()[0].quantity == 50);
    assert(inventory.Slots()[1].itemId == "wood");
    assert(inventory.Slots()[1].quantity == 5);
    assert(inventory.AddItem(database, "wood", 60) == 15);
    assert(inventory.Slots()[0].quantity == 50);
    assert(inventory.Slots()[1].quantity == 50);
    assert(inventory.Contains("wood", 100));
    assert(!inventory.Contains("wood", 101));
    assert(inventory.RemoveItem("wood", 30));
    assert(inventory.TotalQuantity("wood") == 70);
    assert(!inventory.RemoveItem("wood", 100));
    assert(inventory.TotalQuantity("wood") == 70);
    assert(inventory.AddItem(database, "unknown", 4) == 4);

    rw::game::Inventory toolInventory(2);
    assert(toolInventory.AddItem(database, "primitive_tool", 2) == 0);
    assert(toolInventory.Slots()[0].quantity == 1);
    assert(toolInventory.Slots()[1].quantity == 1);

    rw::game::Hotbar hotbar(8);
    assert(hotbar.AssignSlot(0, 0));
    assert(hotbar.AssignSlot(1, 1));
    assert(!hotbar.AssignSlot(8, 0));
    assert(hotbar.SelectedIndex() == 0);
    assert(hotbar.SelectedInventorySlot() == 0);
    hotbar.SelectNext();
    assert(hotbar.SelectedIndex() == 1);
    assert(hotbar.SelectedInventorySlot() == 1);
    hotbar.SelectPrevious();
    assert(hotbar.SelectedIndex() == 0);
    assert(hotbar.SelectIndex(7));
    assert(hotbar.SelectedIndex() == 7);
    assert(!hotbar.SelectIndex(8));
}

