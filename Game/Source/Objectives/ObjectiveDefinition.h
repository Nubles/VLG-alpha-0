#pragma once

#include <string>
#include <vector>

namespace rw::game {

struct ObjectiveRequirement {
    std::string itemId;
    int quantity = 0;
    bool consumeOnCompletion = true;
};

struct ObjectiveDefinition {
    std::string id;
    std::string displayName;
    std::vector<ObjectiveRequirement> requirements;
};

} // namespace rw::game
