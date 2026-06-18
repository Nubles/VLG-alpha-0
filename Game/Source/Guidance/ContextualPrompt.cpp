#include "Game/Source/Guidance/ContextualPrompt.h"

namespace rw::game {

std::string ContextualPrompt::Build(const ContextualPromptState& state)
{
    if (state.buildModeActive) {
        return "ENTER PLACE - Q/R ROTATE - CANCEL";
    }

    if (state.enemyRelevant && !state.enemyName.empty()) {
        return "C ATTACK " + state.enemyName;
    }

    if (!state.hasTarget) {
        return "EXPLORE MISTWOOD HOLLOW";
    }

    if (state.targetIsStabilisedFracture) {
        return "REALM FRACTURE STABILISED";
    }

    if (state.targetRequiresMissingTool) {
        return "REQUIRES " + (state.requiredToolName.empty() ? std::string("TOOL") : state.requiredToolName);
    }

    if (state.targetIsRealmFracture) {
        return "E STABILISE FRACTURE - REQS";
    }

    if (state.targetIsGatherable) {
        return "E GATHER " + state.targetName;
    }

    return "E INSPECT " + state.targetName;
}

} // namespace rw::game
