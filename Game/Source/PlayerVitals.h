#pragma once

namespace rw::game {

class PlayerVitals {
public:
    void Update(float deltaSeconds, bool wantsSprint, bool isMoving);

    float Health() const;
    float Stamina() const;
    bool CanSprint() const;
    bool IsSprinting() const;
    void ApplyDamage(float damage);
    void Heal(float amount);
    void SetHealth(float health);
    void SetStamina(float stamina);
    bool IsAlive() const;

private:
    float m_health = 100.0F;
    float m_stamina = 100.0F;
    float m_regenDelayRemaining = 0.0F;
    bool m_isSprinting = false;
};

} // namespace rw::game
