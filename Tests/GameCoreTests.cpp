#include "Tests/Game/GameTestSuite.h"

int main()
{
    TestDebugSystems();
    TestBiomeDefinitionsAndLayout();
    TestInventoryAndHotbar();
    TestGathering();
    TestCrafting();
    TestBuilding();
    TestCombatAndVitals();
    TestProgressionObjectives();
    TestSaveLoad();
    return 0;
}
