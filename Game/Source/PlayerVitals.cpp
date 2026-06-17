#include "Game/Source/PlayerVitals.h"

#include <algorithm>

namespace rw::game {

namespace {

constexpr float kMaxStamina = 100.0F;
constexpr float kSprintDrainPerSecond = 28.0F;
constexpr float kStaminaRegenPerSecond = 22.0F;
constexpr float kStaminaRegenDelaySeconds = 0.75F;
constexpr float kMinimumSprintStamina = 1.0F;

} // namespace

void PlayerVitals::Update(float deltaSeconds, bool wantsSprint, bool isMoving)
{
    m_isSprinting = wantsSprint && isMoving && m_stamina > kMinimumSprintStamina;

    if (m_isSprinting) {
        m_stamina = std::max(0.0F, m_stamina - kSprintDrainPerSecond * deltaSeconds);
        m_regenDelayRemaining = kStaminaRegenDelaySeconds;
        return;
    }

    if (m_regenDelayRemaining > 0.0F) {
        const float remainingDelta = deltaSeconds - m_regenDelayRemaining;
        m_regenDelayRemaining = std::max(0.0F, m_regenDelayRemaining - deltaSeconds);
        if (remainingDelta <= 0.0F) {
            return;
        }

        deltaSeconds = remainingDelta;
    }

    m_stamina = std::min(kMaxStamina, m_stamina + kStaminaRegenPerSecond * deltaSeconds);
}

float PlayerVitals::Health() const
{
    return m_health;
}

float PlayerVitals::Stamina() const
{
    return m_stamina;
}

bool PlayerVitals::CanSprint() const
{
    return m_stamina > kMinimumSprintStamina;
}

bool PlayerVitals::IsSprinting() const
{
    return m_isSprinting;
}

void PlayerVitals::ApplyDamage(float damage)
{
    m_health = std::max(0.0F, m_health - std::max(0.0F, damage));
}

void PlayerVitals::Heal(float amount)
{
    m_health = std::min(100.0F, m_health + std::max(0.0F, amount));
}

void PlayerVitals::SetHealth(float health)
{
    m_health = std::clamp(health, 0.0F, 100.0F);
}

void PlayerVitals::SetStamina(float stamina)
{
    m_stamina = std::clamp(stamina, 0.0F, 100.0F);
    m_regenDelayRemaining = 0.0F;
    m_isSprinting = false;
}

bool PlayerVitals::IsAlive() const
{
    return m_health > 0.0F;
}

} // namespace rw::game
