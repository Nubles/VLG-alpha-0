#include "Game/Source/UI/HudFormatter.h"

#include <algorithm>

namespace rw::game {

float HudFormatter::ClampedRatio(float current, float maximum)
{
    if (maximum <= 0.0F) {
        return 0.0F;
    }
    return std::clamp(current / maximum, 0.0F, 1.0F);
}

std::string HudFormatter::TargetText(const HudState& state)
{
    return state.currentTargetName.empty() ? "None" : state.currentTargetName;
}

std::string HudFormatter::BuildModeText(const HudState& state)
{
    if (!state.buildModeActive) {
        return "Build: Off";
    }
    return "Build: " + (state.selectedBuildableName.empty() ? std::string("None") : state.selectedBuildableName);
}

std::string HudFormatter::ObjectiveText(const HudState& state)
{
    return state.objectiveStatus.empty() ? "fracture unknown" : state.objectiveStatus;
}

} // namespace rw::game
