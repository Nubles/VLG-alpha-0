#pragma once

#include "Game/Source/UI/HudState.h"

#include <string>

namespace rw::game {

class HudFormatter {
public:
    static float ClampedRatio(float current, float maximum);
    static std::string TargetText(const HudState& state);
    static std::string BuildModeText(const HudState& state);
    static std::string ObjectiveText(const HudState& state);
};

} // namespace rw::game
