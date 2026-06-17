#pragma once

#include "Engine/Math/Transform.h"
#include "Game/Source/AI/EnemyState.h"
#include "Game/Source/Combat/Damageable.h"
#include "Game/Source/Inventory/ItemStack.h"
#include "Game/Source/Resources/DropTable.h"

#include <string>
#include <vector>

namespace rw::game {

class Inventory;
class ItemDatabase;
class PlayerVitals;

struct EnemyUpdateResult {
    bool attackedPlayer = false;
    bool diedThisFrame = false;
    std::string message;
};

struct EnemyLootResult {
    bool dropped = false;
    int leftoverQuantity = 0;
    std::vector<ItemStack> drops;
    std::string message;
};

class EnemyAgent {
public:
    static EnemyAgent CreateRealmWisp();

    EnemyUpdateResult Update(float deltaSeconds, const rw::math::Vec3& playerPosition, PlayerVitals& playerVitals);
    EnemyLootResult DropLoot(const ItemDatabase& items, Inventory& inventory);

    std::string name;
    rw::math::Transform transform;
    Damageable health { 50.0F };
    EnemyState state = EnemyState::Idle;
    DropTable loot;
    float detectionRange = 8.0F;
    float attackRange = 1.5F;
    float attackDamage = 10.0F;
    float attackCooldownSeconds = 1.0F;
    float moveSpeed = 2.0F;
    bool lootDropped = false;

private:
    float m_attackCooldownRemaining = 0.0F;
};

} // namespace rw::game
