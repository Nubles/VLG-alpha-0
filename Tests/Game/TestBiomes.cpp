#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Biomes/BiomeDatabase.h"
#include "Game/Source/Biomes/BiomeLayoutBuilder.h"

#include <cassert>

void TestBiomeDefinitionsAndLayout()
{
    const rw::game::BiomeDatabase biomeDatabase = rw::game::BiomeDatabase::CreateStarterBiomes();
    const rw::game::BiomeDefinition* mistwood = biomeDatabase.FindById("mistwood_hollow");
    assert(mistwood != nullptr);
    assert(mistwood->displayName == "Mistwood Hollow");
    assert(mistwood->difficultyTier == 1);
    assert(biomeDatabase.FindById("missing_biome") == nullptr);
    assert(mistwood->resourceThemeTags.size() == 4);
    assert(mistwood->resourceThemeTags[0] == "wood");
    assert(mistwood->resourceThemeTags[1] == "stone");
    assert(mistwood->resourceThemeTags[2] == "fiber");
    assert(mistwood->resourceThemeTags[3] == "raw_food");
    assert(mistwood->landmarkNames.size() == 3);
    assert(mistwood->landmarkNames[0] == "Old Root Circle");
    assert(mistwood->landmarkNames[1] == "Broken Waystone");
    assert(mistwood->landmarkNames[2] == "Realm Fracture");

    const rw::game::BiomeLayout mistwoodLayout = rw::game::BiomeLayoutBuilder::BuildMistwoodHollow();
    assert(!mistwoodLayout.gatherableNodes.empty());
    assert(!mistwoodLayout.interactables.empty());
    bool hasRealmFracture = false;
    bool hasAncientStone = false;
    for (const rw::game::Interactable& interactable : mistwoodLayout.interactables) {
        if (interactable.name == "Realm Fracture") {
            hasRealmFracture = true;
        }
        if (interactable.name == "Ancient Stone") {
            hasAncientStone = true;
        }
    }
    assert(hasRealmFracture);
    assert(hasAncientStone);
    assert(mistwoodLayout.realmWisp.name == "Realm Wisp");
    assert(mistwoodLayout.realmWisp.health.Health() == 50.0F);
    assert(mistwoodLayout.gatherableNodes[0].id == "branch_pile_01");
}

