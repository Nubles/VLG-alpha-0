#include "Game/Source/Combat/PlayerCombat.h"

#include "Engine/Math/Vec3.h"
#include "Game/Source/Combat/Damageable.h"

#include <algorithm>

namespace rw::game {

namespace {

constexpr float kMeleeDamage = 25.0F;
constexpr float kMeleeRange = 3.0F;
constexpr float kMeleeCooldownSeconds = 0.5F;
constexpr float kMinimumAimDot = 0.65F;

} // namespace

void PlayerCombat::Update(float deltaSeconds)
{
    m_cooldownRemaining = std::max(0.0F, m_cooldownRemaining - deltaSeconds);
}

PlayerAttackResult PlayerCombat::TryMeleeAttack(
    const rw::scene::Camera& camera,
    const rw::math::Vec3& targetPosition,
    Damageable& target)
{
    if (m_cooldownRemaining > 0.0F) {
        return { false, false, true, 0.0F };
    }

    m_cooldownRemaining = kMeleeCooldownSeconds;
    if (!target.IsAlive()) {
        return { true, false, false, 0.0F };
    }

    const rw::math::Vec3 toTarget = targetPosition - camera.position;
    const float distance = rw::math::Length(toTarget);
    const float aimDot = rw::math::Dot(rw::math::Normalize(toTarget), camera.Forward());
    if (distance > kMeleeRange || aimDot < kMinimumAimDot) {
        return { true, false, false, 0.0F };
    }

    target.ApplyDamage({ kMeleeDamage, "Player" });
    return { true, true, false, kMeleeDamage };
}

} // namespace rw::game
