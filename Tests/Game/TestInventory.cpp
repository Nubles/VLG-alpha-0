#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"

#include <cassert>
#include <string>

void TestInventoryAndHotbar()
{
    const std::string validItemData =
        "# id|display_name|description|category|max_stack|tier\n"
        "\n"
        " wood | Wood | A sturdy piece of weathered timber. | Resource | 50 | 0 \n"
        "realm_anchor|Realm Anchor|A crude anchor of stabilized realm energy. It has no world function yet.|Tool|1|0\n";
    const rw::game::ItemDatabaseLoadResult loadedItems = rw::game::ItemDatabase::LoadFromText(validItemData);
    assert(loadedItems.success);
    assert(loadedItems.database.FindById("wood") != nullptr);
    assert(loadedItems.database.FindById("wood")->category == rw::game::ItemCategory::Resource);
    assert(loadedItems.database.FindById("realm_anchor") != nullptr);
    assert(loadedItems.database.FindById("missing_item") == nullptr);
    assert(loadedItems.database.Definitions().size() == 2);

    const rw::game::ItemDatabaseLoadResult invalidCategory = rw::game::ItemDatabase::LoadFromText(
        "bad_item|Bad Item|Invalid category test.|Unknown|1|0\n");
    assert(!invalidCategory.success);
    assert(invalidCategory.message.find("Unknown item category") != std::string::npos);

    const rw::game::ItemDatabaseLoadResult missingField = rw::game::ItemDatabase::LoadFromText(
        "bad_item|Bad Item|Missing fields.|Tool|1\n");
    assert(!missingField.success);
    assert(missingField.message.find("Expected 6 fields") != std::string::npos);

    const rw::game::ItemDatabaseLoadResult invalidStack = rw::game::ItemDatabase::LoadFromText(
        "bad_item|Bad Item|Invalid stack test.|Tool|0|0\n");
    assert(!invalidStack.success);
    assert(invalidStack.message.find("max_stack must be greater than 0") != std::string::npos);

    const rw::game::ItemDatabase fallbackDatabase = rw::game::ItemDatabase::CreateFromFileOrFallback(
        "Game/Data/Items/does_not_exist.txt");
    assert(fallbackDatabase.FindById("wood") != nullptr);
    assert(fallbackDatabase.FindById("realm_anchor") != nullptr);

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
