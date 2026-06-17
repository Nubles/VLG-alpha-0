#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/SaveLoad/SaveSerializer.h"

#include <cassert>
#include <string>

void TestSaveLoad()
{
    rw::game::SaveData saveData;
    saveData.biomeId = "mistwood_hollow";
    saveData.playerPosition = { 1.0F, 2.0F, 3.0F };
    saveData.playerYawRadians = 0.75F;
    saveData.playerPitchRadians = -0.25F;
    saveData.playerHealth = 80.0F;
    saveData.playerStamina = 45.0F;
    saveData.inventorySlots = { { "wood", 12 }, { "fiber", 4 } };
    saveData.progressionFlags = { rw::game::kMistwoodFractureDiscovered, rw::game::kMistwoodShrineCompleted };
    saveData.placedBuildables = { { "placed_0007", "camp_marker", { 2.0F, 0.0F, 1.0F }, 0.5F } };
    saveData.depletedGatherableIds = { "branch_pile_01", "cracked_rock_01" };
    saveData.mistwoodFractureStabilized = true;
    saveData.mistwoodObjectiveCompleted = true;
    saveData.realmWispAlive = false;
    saveData.realmWispPosition = { 4.0F, 0.7F, -5.0F };

    const std::string saveText = rw::game::SaveSerializer::Serialize(saveData);
    assert(saveText.find("biome=mistwood_hollow") != std::string::npos);
    assert(saveText.find("player.position=1,2,3") != std::string::npos);
    assert(saveText.find("player.yaw=0.75") != std::string::npos);
    assert(saveText.find("player.pitch=-0.25") != std::string::npos);
    assert(saveText.find("player.health=80") != std::string::npos);
    assert(saveText.find("player.stamina=45") != std::string::npos);
    assert(saveText.find("inventory.slot.0=wood,12") != std::string::npos);
    assert(saveText.find("progression.flag=mistwood_shrine_completed") != std::string::npos);
    assert(saveText.find("buildable.0=placed_0007,camp_marker,2,0,1,0.5") != std::string::npos);
    assert(saveText.find("gatherable.depleted=branch_pile_01") != std::string::npos);

    rw::game::LoadResult roundTrip = rw::game::SaveSerializer::Deserialize(saveText);
    assert(roundTrip.success);
    assert(roundTrip.data.biomeId == "mistwood_hollow");
    assert(roundTrip.data.playerPosition.x == 1.0F);
    assert(roundTrip.data.playerYawRadians == 0.75F);
    assert(roundTrip.data.playerPitchRadians == -0.25F);
    assert(roundTrip.data.playerHealth == 80.0F);
    assert(roundTrip.data.playerStamina == 45.0F);
    assert(roundTrip.data.inventorySlots[0].itemId == "wood");
    assert(roundTrip.data.inventorySlots[0].quantity == 12);
    assert(roundTrip.data.progressionFlags.size() == 2);
    assert(roundTrip.data.placedBuildables[0].instanceId == "placed_0007");
    assert(roundTrip.data.placedBuildables[0].buildableId == "camp_marker");
    assert(roundTrip.data.placedBuildables[0].position.x == 2.0F);
    assert(roundTrip.data.depletedGatherableIds.size() == 2);
    assert(roundTrip.data.depletedGatherableIds[0] == "branch_pile_01");
    assert(roundTrip.data.mistwoodFractureStabilized);
    assert(roundTrip.data.mistwoodObjectiveCompleted);
    assert(!roundTrip.data.realmWispAlive);

    rw::game::LoadResult invalidSave = rw::game::SaveSerializer::Deserialize("version=1\nnot-valid\n");
    assert(!invalidSave.success);
    rw::game::LoadResult unknownVersion = rw::game::SaveSerializer::Deserialize("version=99\nbiome=mistwood_hollow\n");
    assert(!unknownVersion.success);
}

