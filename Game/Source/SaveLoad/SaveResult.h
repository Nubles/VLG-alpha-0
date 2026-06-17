#pragma once

#include <string>

namespace rw::game {

struct SaveResult {
    bool success = false;
    std::string message;
};

} // namespace rw::game
