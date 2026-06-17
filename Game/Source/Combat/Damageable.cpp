#include "Game/Source/Combat/Damageable.h"

#include <algorithm>

namespace rw::game {

Damageable::Damageable(float maxHealth)
    : m_maxHealth(std::max(0.0F, maxHealth))
    , m_health(m_maxHealth)
{
}

void Damageable::ApplyDamage(const DamageEvent& damage)
{
    m_health = std::max(0.0F, m_health - std::max(0.0F, damage.amount));
}

float Damageable::Health() const
{
    return m_health;
}

float Damageable::MaxHealth() const
{
    return m_maxHealth;
}

bool Damageable::IsAlive() const
{
    return m_health > 0.0F;
}

} // namespace rw::game
