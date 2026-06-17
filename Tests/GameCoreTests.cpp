#include "Game/Source/Building/BuildableDatabase.h"
#include "Game/Source/Building/BuildPlacementController.h"
#include "Game/Source/AI/EnemyAgent.h"
#include "Game/Source/AI/EnemyBrain.h"
#include "Game/Source/Biomes/BiomeDatabase.h"
#include "Game/Source/Biomes/BiomeLayoutBuilder.h"
#include "Game/Source/Combat/Damageable.h"
#include "Game/Source/Combat/PlayerCombat.h"
#include "Game/Source/Crafting/CraftingService.h"
#include "Game/Source/Crafting/RecipeDatabase.h"
#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Objectives/ShrineObjective.h"
#include "Game/Source/PlayerVitals.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/Progression/ProgressionState.h"
#include "Game/Source/Resources/GatherableNode.h"
#include "Game/Source/SaveLoad/SaveSerializer.h"

#include <cassert>

int main()
{
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();

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

    rw::game::SaveData saveData;
    saveData.biomeId = "mistwood_hollow";
    saveData.playerPosition = { 1.0F, 2.0F, 3.0F };
    saveData.playerHealth = 80.0F;
    saveData.playerStamina = 45.0F;
    saveData.inventorySlots = { { "wood", 12 }, { "fiber", 4 } };
    saveData.progressionFlags = { rw::game::kMistwoodFractureDiscovered, rw::game::kMistwoodShrineCompleted };
    saveData.placedBuildables = { { "camp_marker", { 2.0F, 0.0F, 1.0F }, 0.5F } };
    saveData.mistwoodFractureStabilized = true;
    saveData.mistwoodObjectiveCompleted = true;
    saveData.realmWispAlive = false;
    saveData.realmWispPosition = { 4.0F, 0.7F, -5.0F };

    const std::string saveText = rw::game::SaveSerializer::Serialize(saveData);
    assert(saveText.find("biome=mistwood_hollow") != std::string::npos);
    assert(saveText.find("player.position=1,2,3") != std::string::npos);
    assert(saveText.find("player.health=80") != std::string::npos);
    assert(saveText.find("player.stamina=45") != std::string::npos);
    assert(saveText.find("inventory.slot.0=wood,12") != std::string::npos);
    assert(saveText.find("progression.flag=mistwood_shrine_completed") != std::string::npos);
    assert(saveText.find("buildable.0=camp_marker,2,0,1,0.5") != std::string::npos);

    rw::game::LoadResult roundTrip = rw::game::SaveSerializer::Deserialize(saveText);
    assert(roundTrip.success);
    assert(roundTrip.data.biomeId == "mistwood_hollow");
    assert(roundTrip.data.playerPosition.x == 1.0F);
    assert(roundTrip.data.playerHealth == 80.0F);
    assert(roundTrip.data.playerStamina == 45.0F);
    assert(roundTrip.data.inventorySlots[0].itemId == "wood");
    assert(roundTrip.data.inventorySlots[0].quantity == 12);
    assert(roundTrip.data.progressionFlags.size() == 2);
    assert(roundTrip.data.placedBuildables[0].buildableId == "camp_marker");
    assert(roundTrip.data.placedBuildables[0].position.x == 2.0F);
    assert(roundTrip.data.mistwoodFractureStabilized);
    assert(roundTrip.data.mistwoodObjectiveCompleted);
    assert(!roundTrip.data.realmWispAlive);

    rw::game::LoadResult invalidSave = rw::game::SaveSerializer::Deserialize("version=1\nnot-valid\n");
    assert(!invalidSave.success);
    rw::game::LoadResult unknownVersion = rw::game::SaveSerializer::Deserialize("version=99\nbiome=mistwood_hollow\n");
    assert(!unknownVersion.success);

    const rw::game::ItemDefinition* wood = database.FindById("wood");
    assert(wood != nullptr);
    assert(wood->displayName == "Wood");
    assert(wood->maxStackSize == 50);
    assert(database.FindById("missing_item") == nullptr);
    assert(database.FindById("camp_bundle") != nullptr);
    assert(database.FindById("simple_meal") != nullptr);
    assert(database.FindById("workbench_kit") != nullptr);
    assert(database.FindById("realm_anchor") != nullptr);

    const rw::game::RecipeDatabase recipes = rw::game::RecipeDatabase::CreateStarterRecipes();
    assert(recipes.FindById("primitive_tool") != nullptr);
    assert(recipes.FindById("camp_bundle") != nullptr);
    assert(recipes.FindById("simple_meal") != nullptr);
    assert(recipes.FindById("workbench_kit") != nullptr);
    assert(recipes.FindById("realm_anchor") != nullptr);
    assert(recipes.FindById("missing_recipe") == nullptr);

    rw::game::ProgressionState progression;
    assert(!progression.HasFlag("unknown_flag"));
    progression.SetFlag(rw::game::kMistwoodFractureDiscovered);
    assert(progression.HasFlag(rw::game::kMistwoodFractureDiscovered));
    progression.Clear();
    assert(!progression.HasFlag(rw::game::kMistwoodFractureDiscovered));

    const rw::game::BuildableDatabase buildables = rw::game::BuildableDatabase::CreateStarterBuildables();
    assert(buildables.FindById("camp_marker") != nullptr);
    assert(buildables.FindById("workbench_stub") != nullptr);
    assert(buildables.FindById("simple_wall") != nullptr);
    assert(buildables.FindById("missing_buildable") == nullptr);

    rw::game::Damageable damageable(50.0F);
    assert(damageable.Health() == 50.0F);
    assert(damageable.IsAlive());
    damageable.ApplyDamage({ 20.0F, "test" });
    assert(damageable.Health() == 30.0F);
    damageable.ApplyDamage({ 100.0F, "test" });
    assert(damageable.Health() == 0.0F);
    assert(!damageable.IsAlive());

    rw::game::PlayerVitals vitals;
    vitals.ApplyDamage(25.0F);
    assert(vitals.Health() == 75.0F);
    assert(vitals.IsAlive());
    vitals.ApplyDamage(100.0F);
    assert(vitals.Health() == 0.0F);
    assert(!vitals.IsAlive());

    assert(rw::game::EnemyBrain::Evaluate(true, 4.0F, 8.0F, 1.5F) == rw::game::EnemyState::Chase);
    assert(rw::game::EnemyBrain::Evaluate(true, 9.0F, 8.0F, 1.5F) == rw::game::EnemyState::Idle);
    assert(rw::game::EnemyBrain::Evaluate(true, 1.0F, 8.0F, 1.5F) == rw::game::EnemyState::Attack);
    assert(rw::game::EnemyBrain::Evaluate(false, 1.0F, 8.0F, 1.5F) == rw::game::EnemyState::Dead);

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

    rw::game::DropTable dropTable;
    dropTable.AddDrop("wood", 4, 4);
    const std::vector<rw::game::ItemStack> drops = dropTable.RollDeterministic();
    assert(drops.size() == 1);
    assert(drops[0].itemId == "wood");
    assert(drops[0].quantity == 4);

    rw::game::Inventory gatheringInventory(4);
    rw::game::GatherableNode branchPile;
    branchPile.name = "Branch Pile";
    branchPile.remainingGatherCount = 1;
    branchPile.dropTable.AddDrop("wood", 3, 3);

    rw::game::GatheringResult branchResult = branchPile.Gather(database, gatheringInventory);
    assert(branchResult.success);
    assert(gatheringInventory.TotalQuantity("wood") == 3);
    assert(branchPile.IsDepleted());

    rw::game::GatheringResult depletedResult = branchPile.Gather(database, gatheringInventory);
    assert(!depletedResult.success);
    assert(gatheringInventory.TotalQuantity("wood") == 3);

    rw::game::Inventory toolCheckInventory(4);
    rw::game::GatherableNode crackedRock;
    crackedRock.name = "Cracked Rock";
    crackedRock.remainingGatherCount = 1;
    crackedRock.requiredToolId = "primitive_tool";
    crackedRock.dropTable.AddDrop("stone", 8, 8);

    rw::game::GatheringResult missingTool = crackedRock.Gather(database, toolCheckInventory);
    assert(!missingTool.success);
    assert(toolCheckInventory.TotalQuantity("stone") == 0);
    assert(!crackedRock.IsDepleted());

    assert(toolCheckInventory.AddItem(database, "primitive_tool", 1) == 0);
    rw::game::GatheringResult withTool = crackedRock.Gather(database, toolCheckInventory);
    assert(withTool.success);
    assert(toolCheckInventory.TotalQuantity("stone") == 8);
    assert(crackedRock.IsDepleted());

    rw::game::Inventory overflowInventory(1);
    rw::game::GatherableNode largePile;
    largePile.name = "Large Branch Pile";
    largePile.remainingGatherCount = 1;
    largePile.dropTable.AddDrop("wood", 60, 60);
    rw::game::GatheringResult overflow = largePile.Gather(database, overflowInventory);
    assert(overflow.success);
    assert(overflowInventory.TotalQuantity("wood") == 50);
    assert(overflow.leftoverQuantity == 10);

    rw::game::Inventory unknownRecipeInventory(4);
    rw::game::CraftingResult unknownRecipe = rw::game::CraftingService::Craft(
        recipes, database, unknownRecipeInventory, "missing_recipe");
    assert(!unknownRecipe.success);
    assert(unknownRecipe.failureReason == rw::game::CraftingFailureReason::UnknownRecipe);

    rw::game::Inventory missingIngredientInventory(4);
    assert(missingIngredientInventory.AddItem(database, "wood", 1) == 0);
    rw::game::CraftingResult missingIngredient = rw::game::CraftingService::Craft(
        recipes, database, missingIngredientInventory, "primitive_tool");
    assert(!missingIngredient.success);
    assert(missingIngredient.failureReason == rw::game::CraftingFailureReason::MissingIngredient);
    assert(missingIngredientInventory.TotalQuantity("wood") == 1);
    assert(missingIngredientInventory.TotalQuantity("primitive_tool") == 0);

    rw::game::Inventory craftingInventory(4);
    assert(craftingInventory.AddItem(database, "wood", 1) == 0);
    assert(craftingInventory.AddItem(database, "stone", 1) == 0);
    assert(craftingInventory.AddItem(database, "fiber", 1) == 0);
    rw::game::CraftingResult primitiveTool = rw::game::CraftingService::Craft(
        recipes, database, craftingInventory, "primitive_tool");
    assert(primitiveTool.success);
    assert(primitiveTool.craftedItemId == "primitive_tool");
    assert(primitiveTool.craftedQuantity == 1);
    assert(craftingInventory.TotalQuantity("wood") == 0);
    assert(craftingInventory.TotalQuantity("stone") == 0);
    assert(craftingInventory.TotalQuantity("fiber") == 0);
    assert(craftingInventory.TotalQuantity("primitive_tool") == 1);

    rw::game::Inventory fullInventory(1);
    assert(fullInventory.AddItem(database, "wood", 10) == 0);
    rw::game::CraftingResult fullResult = rw::game::CraftingService::Craft(
        recipes, database, fullInventory, "camp_bundle");
    assert(!fullResult.success);
    assert(fullResult.failureReason == rw::game::CraftingFailureReason::MissingIngredient);
    assert(fullInventory.TotalQuantity("wood") == 10);
    assert(fullInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::Inventory outputBlockedInventory(2);
    assert(outputBlockedInventory.AddItem(database, "wood", 50) == 0);
    assert(outputBlockedInventory.AddItem(database, "fiber", 50) == 0);
    rw::game::CraftingResult outputBlocked = rw::game::CraftingService::Craft(
        recipes, database, outputBlockedInventory, "camp_bundle");
    assert(!outputBlocked.success);
    assert(outputBlocked.failureReason == rw::game::CraftingFailureReason::InventoryFull);
    assert(outputBlocked.leftoverOutputQuantity == 1);
    assert(outputBlockedInventory.TotalQuantity("wood") == 50);
    assert(outputBlockedInventory.TotalQuantity("fiber") == 50);
    assert(outputBlockedInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::ProgressionState objectiveProgression;
    rw::game::ObjectiveState objectiveState;
    rw::game::Inventory objectiveMissingInventory(8);
    assert(objectiveMissingInventory.AddItem(database, "camp_bundle", 1) == 0);
    rw::game::ObjectiveResult objectiveMissing = rw::game::ShrineObjective::TryCompleteMistwoodFracture(
        objectiveMissingInventory, objectiveProgression, objectiveState);
    assert(!objectiveMissing.success);
    assert(objectiveMissing.failureReason == rw::game::ObjectiveFailureReason::MissingRequirement);
    assert(objectiveMissingInventory.TotalQuantity("camp_bundle") == 1);
    assert(objectiveProgression.HasFlag(rw::game::kMistwoodFractureDiscovered));
    assert(!objectiveProgression.HasFlag(rw::game::kMistwoodShrineCompleted));

    rw::game::ProgressionState completeProgression;
    rw::game::ObjectiveState completeObjectiveState;
    rw::game::Inventory objectiveInventory(8);
    assert(objectiveInventory.AddItem(database, "camp_bundle", 1) == 0);
    assert(objectiveInventory.AddItem(database, "primitive_tool", 1) == 0);
    assert(objectiveInventory.AddItem(database, "fiber", 3) == 0);
    rw::game::ObjectiveResult objectiveComplete = rw::game::ShrineObjective::TryCompleteMistwoodFracture(
        objectiveInventory, completeProgression, completeObjectiveState);
    assert(objectiveComplete.success);
    assert(objectiveInventory.TotalQuantity("camp_bundle") == 0);
    assert(objectiveInventory.TotalQuantity("fiber") == 0);
    assert(objectiveInventory.TotalQuantity("primitive_tool") == 1);
    assert(completeProgression.HasFlag(rw::game::kMistwoodShrineCompleted));
    assert(completeProgression.HasFlag(rw::game::kMistwoodFractureStabilized));
    assert(completeProgression.HasFlag(rw::game::kRecipeRealmAnchorUnlocked));

    rw::game::Inventory lockedAnchorInventory(8);
    assert(lockedAnchorInventory.AddItem(database, "workbench_kit", 1) == 0);
    assert(lockedAnchorInventory.AddItem(database, "fiber", 5) == 0);
    assert(lockedAnchorInventory.AddItem(database, "stone", 5) == 0);
    rw::game::CraftingResult lockedAnchor = rw::game::CraftingService::Craft(
        recipes, database, lockedAnchorInventory, "realm_anchor");
    assert(!lockedAnchor.success);
    assert(lockedAnchor.failureReason == rw::game::CraftingFailureReason::Locked);
    assert(lockedAnchorInventory.TotalQuantity("workbench_kit") == 1);

    rw::game::ProgressionState anchorProgression;
    anchorProgression.SetFlag(rw::game::kRecipeRealmAnchorUnlocked);
    rw::game::Inventory unlockedAnchorInventory(8);
    assert(unlockedAnchorInventory.AddItem(database, "workbench_kit", 1) == 0);
    assert(unlockedAnchorInventory.AddItem(database, "fiber", 5) == 0);
    assert(unlockedAnchorInventory.AddItem(database, "stone", 5) == 0);
    rw::game::CraftingResult unlockedAnchor = rw::game::CraftingService::Craft(
        recipes, database, anchorProgression, unlockedAnchorInventory, "realm_anchor");
    assert(unlockedAnchor.success);
    assert(unlockedAnchorInventory.TotalQuantity("realm_anchor") == 1);

    rw::scene::Camera buildCamera;
    buildCamera.position = { 0.0F, 1.6F, 0.0F };
    buildCamera.yawRadians = 0.0F;
    buildCamera.pitchRadians = 0.0F;

    rw::game::BuildPlacementController missingCostBuild;
    missingCostBuild.SelectBuildable("camp_marker");
    missingCostBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory missingBuildInventory(4);
    const std::vector<rw::game::PlacedBuildable> noPlacedBuildables;
    rw::game::BuildPlacementResult missingBuildCost = missingCostBuild.TryPlace(
        buildables, database, missingBuildInventory, noPlacedBuildables);
    assert(!missingBuildCost.success);
    assert(missingBuildCost.failureReason == rw::game::BuildPlacementFailureReason::MissingCost);
    assert(missingBuildInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::BuildPlacementController campBuild;
    campBuild.SelectBuildable("camp_marker");
    campBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory campBuildInventory(4);
    assert(campBuildInventory.AddItem(database, "camp_bundle", 1) == 0);
    rw::game::BuildPlacementResult campPlaced = campBuild.TryPlace(
        buildables, database, campBuildInventory, noPlacedBuildables);
    assert(campPlaced.success);
    assert(campPlaced.placed.buildableId == "camp_marker");
    assert(campBuildInventory.TotalQuantity("camp_bundle") == 0);

    rw::game::BuildPlacementController workbenchBuild;
    workbenchBuild.SelectBuildable("workbench_stub");
    workbenchBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory workbenchInventory(4);
    assert(workbenchInventory.AddItem(database, "workbench_kit", 1) == 0);
    rw::game::BuildPlacementResult workbenchPlaced = workbenchBuild.TryPlace(
        buildables, database, workbenchInventory, noPlacedBuildables);
    assert(workbenchPlaced.success);
    assert(workbenchInventory.TotalQuantity("workbench_kit") == 0);

    rw::game::BuildPlacementController wallBuild;
    wallBuild.SelectBuildable("simple_wall");
    wallBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory wallInventory(4);
    assert(wallInventory.AddItem(database, "wood", 3) == 0);
    assert(wallInventory.AddItem(database, "fiber", 1) == 0);
    rw::game::BuildPlacementResult wallPlaced = wallBuild.TryPlace(
        buildables, database, wallInventory, noPlacedBuildables);
    assert(wallPlaced.success);
    assert(wallInventory.TotalQuantity("wood") == 0);
    assert(wallInventory.TotalQuantity("fiber") == 0);

    rw::game::BuildableDatabase invalidBuildables;
    invalidBuildables.AddBuildable({
        "too_close",
        "Too Close",
        { { "wood", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 1.0F, 1.0F, 1.0F },
        { 1.0F, 1.0F, 1.0F },
        0.5F,
        0.75F,
        "Tests",
    });
    rw::game::BuildPlacementController invalidBuild;
    invalidBuild.SelectBuildable("too_close");
    invalidBuild.UpdatePreview(invalidBuildables, buildCamera);
    rw::game::Inventory invalidBuildInventory(4);
    assert(invalidBuildInventory.AddItem(database, "wood", 1) == 0);
    rw::game::BuildPlacementResult invalidPlacement = invalidBuild.TryPlace(
        invalidBuildables, database, invalidBuildInventory, noPlacedBuildables);
    assert(!invalidPlacement.success);
    assert(invalidPlacement.failureReason == rw::game::BuildPlacementFailureReason::InvalidPlacement);
    assert(invalidBuildInventory.TotalQuantity("wood") == 1);

    rw::game::BuildPlacementController overlapBuild;
    overlapBuild.SelectBuildable("camp_marker");
    overlapBuild.UpdatePreview(buildables, buildCamera);
    rw::game::Inventory overlapInventory(4);
    assert(overlapInventory.AddItem(database, "camp_bundle", 1) == 0);
    const std::vector<rw::game::PlacedBuildable> placedForOverlap { campPlaced.placed };
    rw::game::BuildPlacementResult overlapResult = overlapBuild.TryPlace(
        buildables, database, overlapInventory, placedForOverlap);
    assert(!overlapResult.success);
    assert(overlapResult.failureReason == rw::game::BuildPlacementFailureReason::Overlap);
    assert(overlapInventory.TotalQuantity("camp_bundle") == 1);

    rw::scene::Camera combatCamera;
    combatCamera.position = { 0.0F, 0.0F, 0.0F };
    combatCamera.yawRadians = 0.0F;
    combatCamera.pitchRadians = 0.0F;

    rw::game::PlayerCombat playerCombat;
    rw::game::Damageable closeEnemy(50.0F);
    rw::game::PlayerAttackResult closeAttack = playerCombat.TryMeleeAttack(
        combatCamera, { 0.0F, 0.0F, 2.0F }, closeEnemy);
    assert(closeAttack.attacked);
    assert(closeAttack.hit);
    assert(closeEnemy.Health() == 25.0F);

    rw::game::PlayerAttackResult cooldownAttack = playerCombat.TryMeleeAttack(
        combatCamera, { 0.0F, 0.0F, 2.0F }, closeEnemy);
    assert(cooldownAttack.onCooldown);

    playerCombat.Update(0.6F);
    rw::game::Damageable farEnemy(50.0F);
    rw::game::PlayerAttackResult missAttack = playerCombat.TryMeleeAttack(
        combatCamera, { 0.0F, 0.0F, 6.0F }, farEnemy);
    assert(missAttack.attacked);
    assert(!missAttack.hit);
    assert(farEnemy.Health() == 50.0F);

    rw::game::EnemyAgent attackingEnemy = rw::game::EnemyAgent::CreateRealmWisp();
    attackingEnemy.transform.position = { 0.0F, 0.0F, 0.5F };
    rw::game::PlayerVitals attackedVitals;
    rw::game::EnemyUpdateResult firstEnemyAttack = attackingEnemy.Update(
        0.1F, { 0.0F, 0.0F, 0.0F }, attackedVitals);
    assert(firstEnemyAttack.attackedPlayer);
    assert(attackedVitals.Health() == 90.0F);
    rw::game::EnemyUpdateResult blockedEnemyAttack = attackingEnemy.Update(
        0.1F, { 0.0F, 0.0F, 0.0F }, attackedVitals);
    assert(!blockedEnemyAttack.attackedPlayer);
    assert(attackedVitals.Health() == 90.0F);

    rw::game::EnemyAgent deadEnemy = rw::game::EnemyAgent::CreateRealmWisp();
    deadEnemy.health.ApplyDamage({ 50.0F, "test" });
    rw::game::Inventory lootInventory(4);
    rw::game::EnemyLootResult lootResult = deadEnemy.DropLoot(database, lootInventory);
    assert(lootResult.dropped);
    assert(!lootResult.drops.empty());
    assert(lootInventory.TotalQuantity("fiber") == 1);

    return 0;
}
