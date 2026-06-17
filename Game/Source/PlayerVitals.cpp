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
        m_regenDelayRemaining = std::max(0.0F, m_regenDelayRemaining - deltaSeconds);
        return;
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

} // namespace rw::game
