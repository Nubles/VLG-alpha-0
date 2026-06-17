#include "Game/Source/AI/EnemyAgent.h"

#include "Engine/Math/Vec3.h"
#include "Game/Source/AI/EnemyBrain.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/PlayerVitals.h"

#include <algorithm>
#include <sstream>

namespace rw::game {

EnemyAgent EnemyAgent::CreateRealmWisp()
{
    EnemyAgent enemy;
    enemy.name = "Realm Wisp";
    enemy.transform.position = { 0.0F, 0.7F, -4.0F };
    enemy.transform.scale = { 0.75F, 0.75F, 0.75F };
    enemy.health = Damageable(50.0F);
    enemy.loot.AddDrop("fiber", 1, 1);
    return enemy;
}

EnemyUpdateResult EnemyAgent::Update(
    float deltaSeconds,
    const rw::math::Vec3& playerPosition,
    PlayerVitals& playerVitals)
{
    m_attackCooldownRemaining = std::max(0.0F, m_attackCooldownRemaining - deltaSeconds);

    const float distance = rw::math::Length(playerPosition - transform.position);
    state = EnemyBrain::Evaluate(health.IsAlive(), distance, detectionRange, attackRange);

    if (state == EnemyState::Dead) {
        return { false, false, {} };
    }

    if (state == EnemyState::Chase) {
        const rw::math::Vec3 direction = rw::math::Normalize(playerPosition - transform.position);
        transform.position += direction * moveSpeed * deltaSeconds;
        return { false, false, name + " chasing" };
    }

    if (state == EnemyState::Attack && m_attackCooldownRemaining <= 0.0F) {
        playerVitals.ApplyDamage(attackDamage);
        m_attackCooldownRemaining = attackCooldownSeconds;
        return { true, false, name + " hit player" };
    }

    return { false, false, {} };
}

EnemyLootResult EnemyAgent::DropLoot(const ItemDatabase& items, Inventory& inventory)
{
    if (lootDropped || health.IsAlive()) {
        return { false, 0, {}, {} };
    }

    lootDropped = true;
    std::vector<ItemStack> drops = loot.RollDeterministic();
    int leftover = 0;
    for (const ItemStack& drop : drops) {
        leftover += inventory.AddItem(items, drop.itemId, drop.quantity);
    }

    std::ostringstream message;
    message << name << " defeated";
    if (!drops.empty()) {
        message << " drops";
        for (const ItemStack& drop : drops) {
            message << " " << drop.itemId << "x" << drop.quantity;
        }
    }
    if (leftover > 0) {
        message << " leftover:" << leftover;
    }

    return { true, leftover, drops, message.str() };
}

} // namespace rw::game
