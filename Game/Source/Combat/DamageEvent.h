#pragma once

#include <string>

namespace rw::game {

struct DamageEvent {
    float amount = 0.0F;
    std::string source;
};

} // namespace rw::game
