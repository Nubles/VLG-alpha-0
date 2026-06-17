#pragma once

#include "Engine/Scene/Camera.h"
#include "Game/Source/Combat/DamageEvent.h"

namespace rw::game {

class Damageable;

struct PlayerAttackResult {
    bool attacked = false;
    bool hit = false;
    bool onCooldown = false;
    float damage = 0.0F;
};

class PlayerCombat {
public:
    void Update(float deltaSeconds);
    PlayerAttackResult TryMeleeAttack(
        const rw::scene::Camera& camera,
        const rw::math::Vec3& targetPosition,
        Damageable& target);

private:
    float m_cooldownRemaining = 0.0F;
};

} // namespace rw::game
