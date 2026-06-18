#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Debug/DebugFormatting.h"
#include "Game/Source/Debug/DebugHelp.h"
#include "Game/Source/Debug/DebugMessageLog.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"

#include <cassert>
#include <string>

void TestDebugSystems()
{
    const std::string helpText = rw::game::DebugHelpText();
    assert(helpText.find("WASD") != std::string::npos);
    assert(helpText.find("E interact") != std::string::npos);
    assert(helpText.find("C attack") != std::string::npos);
    assert(helpText.find("O save") != std::string::npos);
    assert(helpText.find("P load") != std::string::npos);
    assert(helpText.find("M toggle mouse look") != std::string::npos);
    assert(helpText.find("H help") != std::string::npos);

    rw::game::DebugMessageLog messageLog(3);
    messageLog.Push("one");
    messageLog.Push("two");
    messageLog.Push("three");
    messageLog.Push("four");
    assert(messageLog.Messages().size() == 3);
    assert(messageLog.Messages()[0] == "two");
    assert(messageLog.Latest() == "four");
    assert(messageLog.Summary().find("two") != std::string::npos);
    assert(messageLog.Summary().find("one") == std::string::npos);

    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    const rw::game::Inventory inventory(2);
    assert(rw::game::CompactInventorySummary(database, inventory) == "empty");
}
