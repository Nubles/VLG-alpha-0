#pragma once

#include <string>

namespace rw::game {

struct ContextualPromptState {
    std::string targetName;
    bool hasTarget = false;
    bool targetIsGatherable = false;
    bool targetIsRealmFracture = false;
    bool targetIsStabilisedFracture = false;
    bool targetRequiresMissingTool = false;
    std::string requiredToolName;
    bool buildModeActive = false;
    bool enemyRelevant = false;
    std::string enemyName;
};

class ContextualPrompt {
public:
    static std::string Build(const ContextualPromptState& state);
};

} // namespace rw::game
