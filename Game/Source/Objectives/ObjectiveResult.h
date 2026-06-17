#pragma once

#include <string>

namespace rw::game {

enum class ObjectiveFailureReason {
    None,
    AlreadyCompleted,
    MissingRequirement
};

struct ObjectiveResult {
    bool success = false;
    ObjectiveFailureReason failureReason = ObjectiveFailureReason::None;
    std::string message;
};

} // namespace rw::game
