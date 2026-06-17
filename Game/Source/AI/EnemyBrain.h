#pragma once

#include "Game/Source/AI/EnemyState.h"

namespace rw::game {

class EnemyBrain {
public:
    static EnemyState Evaluate(
        bool isAlive,
        float distanceToPlayer,
        float detectionRange,
        float attackRange);
};

} // namespace rw::game
