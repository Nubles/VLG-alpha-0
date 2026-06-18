#pragma once

#include <string>

namespace rw::game {

struct VerticalSliceGuideState {
    bool mouseLookEnabled = false;
    int wood = 0;
    int stone = 0;
    int fiber = 0;
    int primitiveTool = 0;
    int campBundle = 0;
    int workbenchKit = 0;
    int realmAnchor = 0;
    bool crackedRockGathered = false;
    bool placedCampMarker = false;
    bool placedWorkbenchStub = false;
    bool realmWispDefeated = false;
    bool fractureStabilised = false;
};

class VerticalSliceGuide {
public:
    static std::string NextStep(const VerticalSliceGuideState& state);
};

} // namespace rw::game
