#include "Tests/Game/GameTestSuite.h"

int main()
{
    TestDebugSystems();
    TestDataTextParser();
    TestBiomeDefinitionsAndLayout();
    TestInventoryAndHotbar();
    TestGathering();
    TestGuidance();
    TestCrafting();
    TestBuilding();
    TestCombatAndVitals();
    TestProgressionObjectives();
    TestSaveLoad();
    TestUI();
    return 0;
}
