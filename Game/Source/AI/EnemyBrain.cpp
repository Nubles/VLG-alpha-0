#include "Game/Source/AI/EnemyBrain.h"

namespace rw::game {

EnemyState EnemyBrain::Evaluate(
    bool isAlive,
    float distanceToPlayer,
    float detectionRange,
    float attackRange)
{
    if (!isAlive) {
        return EnemyState::Dead;
    }
    if (distanceToPlayer <= attackRange) {
        return EnemyState::Attack;
    }
    if (distanceToPlayer <= detectionRange) {
        return EnemyState::Chase;
    }
    return EnemyState::Idle;
}

const char* ToString(EnemyState state)
{
    switch (state) {
    case EnemyState::Idle:
        return "Idle";
    case EnemyState::Chase:
        return "Chase";
    case EnemyState::Attack:
        return "Attack";
    case EnemyState::Dead:
        return "Dead";
    default:
        return "Unknown";
    }
}

} // namespace rw::game
