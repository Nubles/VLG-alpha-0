#pragma once

#include "Game/Source/Combat/DamageEvent.h"

namespace rw::game {

class Damageable {
public:
    explicit Damageable(float maxHealth = 100.0F);

    void ApplyDamage(const DamageEvent& damage);
    float Health() const;
    float MaxHealth() const;
    bool IsAlive() const;

private:
    float m_maxHealth = 100.0F;
    float m_health = 100.0F;
};

} // namespace rw::game
