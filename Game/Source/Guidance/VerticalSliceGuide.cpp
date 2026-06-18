#include "Game/Source/Guidance/VerticalSliceGuide.h"

namespace rw::game {

std::string VerticalSliceGuide::NextStep(const VerticalSliceGuideState& state)
{
    if (!state.mouseLookEnabled) {
        return "NEXT: PRESS M FOR MOUSE LOOK";
    }
    if (state.wood < 1 || state.stone < 1 || state.fiber < 1) {
        return "NEXT: GATHER WOOD STONE FIBER";
    }
    if (state.primitiveTool < 1) {
        return "NEXT: CRAFT PRIMITIVE_TOOL";
    }
    if (!state.crackedRockGathered) {
        return "NEXT: GATHER CRACKED ROCK";
    }
    if (state.campBundle < 1 || state.workbenchKit < 1) {
        return "NEXT: CRAFT CAMP_BUNDLE WORKBENCH_KIT";
    }
    if (!state.placedCampMarker && !state.placedWorkbenchStub) {
        return "NEXT: PLACE CAMP OR WORKBENCH";
    }
    if (!state.realmWispDefeated) {
        return "NEXT: DEFEAT REALM WISP";
    }
    if (!state.fractureStabilised) {
        return "NEXT: STABILISE FRACTURE";
    }
    if (state.realmAnchor < 1) {
        return "NEXT: CRAFT REALM_ANCHOR";
    }
    return "NEXT: SAVE O / LOAD P";
}

} // namespace rw::game
