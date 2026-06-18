#include "Tests/Game/GameTestSuite.h"

#include "Engine/Scene/Camera.h"
#include "Game/Source/AI/EnemyAgent.h"
#include "Game/Source/AI/EnemyBrain.h"
#include "Game/Source/Combat/Damageable.h"
#include "Game/Source/Combat/PlayerCombat.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/PlayerController.h"
#include "Game/Source/PlayerVitals.h"

#include <cassert>

void TestCombatAndVitals()
{
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

    rw::game::PlayerController player;
    const float startYaw = player.Camera().yawRadians;
    const float startPitch = player.Camera().pitchRadians;

    rw::input::InputState mouseInput;
    mouseInput.SetMouseCaptureEnabled(true);
    mouseInput.AddMouseDelta(20, -10);
    player.ApplyLookInput(0.0F, mouseInput);
    assert(player.Camera().yawRadians < startYaw);
    assert(player.Camera().pitchRadians > startPitch);

    const float mouseYaw = player.Camera().yawRadians;
    const float mousePitch = player.Camera().pitchRadians;
    rw::input::InputState zeroMouseInput;
    zeroMouseInput.SetMouseCaptureEnabled(true);
    player.ApplyLookInput(0.0F, zeroMouseInput);
    assert(player.Camera().yawRadians == mouseYaw);
    assert(player.Camera().pitchRadians == mousePitch);

    rw::input::InputState arrowInput;
    arrowInput.SetKeyDown(rw::input::Key::Left, true);
    player.ApplyLookInput(1.0F, arrowInput);
    assert(player.Camera().yawRadians > mouseYaw);

    player.SetLook(0.0F, 0.0F);
    rw::input::InputState clampInput;
    clampInput.SetMouseCaptureEnabled(true);
    clampInput.AddMouseDelta(0, -10000);
    player.ApplyLookInput(0.0F, clampInput);
    assert(player.Camera().pitchRadians <= 1.45F);

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
    const rw::game::ItemDatabase database = rw::game::ItemDatabase::CreateStarterDatabase();
    rw::game::Inventory lootInventory(4);
    rw::game::EnemyLootResult lootResult = deadEnemy.DropLoot(database, lootInventory);
    assert(lootResult.dropped);
    assert(!lootResult.drops.empty());
    assert(lootInventory.TotalQuantity("fiber") == 1);
}
