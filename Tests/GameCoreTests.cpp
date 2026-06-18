#include "Tests/Game/GameTestSuite.h"

int main()
{
    TestDebugSystems();
    TestDataTextParser();
    TestBiomeDefinitionsAndLayout();
    TestInventoryAndHotbar();
    TestGathering();
    TestCrafting();
    TestBuilding();
    TestCombatAndVitals();
    TestProgressionObjectives();
    TestSaveLoad();
    TestUI();
    return 0;
}
