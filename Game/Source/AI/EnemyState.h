#pragma once

namespace rw::game {

enum class EnemyState {
    Idle,
    Chase,
    Attack,
    Dead
};

const char* ToString(EnemyState state);

} // namespace rw::game
