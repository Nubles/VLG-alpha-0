#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Guidance/ContextualPrompt.h"
#include "Game/Source/Guidance/VerticalSliceGuide.h"

#include <cassert>
#include <string>

namespace {

void AssertShort(const std::string& text)
{
    assert(text.size() <= 48);
}

} // namespace

void TestGuidance()
{
    rw::game::ContextualPromptState prompt;
    assert(rw::game::ContextualPrompt::Build(prompt) == "EXPLORE MISTWOOD HOLLOW");

    prompt.hasTarget = true;
    prompt.targetIsGatherable = true;
    prompt.targetName = "Branch Pile";
    assert(rw::game::ContextualPrompt::Build(prompt) == "E GATHER Branch Pile");

    prompt.targetRequiresMissingTool = true;
    prompt.requiredToolName = "Primitive Tool";
    assert(rw::game::ContextualPrompt::Build(prompt) == "REQUIRES Primitive Tool");

    prompt = {};
    prompt.hasTarget = true;
    prompt.targetName = "Realm Fracture";
    prompt.targetIsRealmFracture = true;
    assert(rw::game::ContextualPrompt::Build(prompt) == "E STABILISE FRACTURE - REQS");

    prompt.targetIsStabilisedFracture = true;
    assert(rw::game::ContextualPrompt::Build(prompt) == "REALM FRACTURE STABILISED");

    prompt = {};
    prompt.buildModeActive = true;
    assert(rw::game::ContextualPrompt::Build(prompt) == "ENTER PLACE - Q/R ROTATE - CANCEL");

    prompt = {};
    prompt.enemyRelevant = true;
    prompt.enemyName = "Realm Wisp";
    assert(rw::game::ContextualPrompt::Build(prompt) == "C ATTACK Realm Wisp");

    prompt.hasTarget = true;
    prompt.targetIsGatherable = true;
    prompt.targetName = "Branch Pile";
    assert(rw::game::ContextualPrompt::Build(prompt) == "E GATHER Branch Pile");

    rw::game::VerticalSliceGuideState guide;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: PRESS M FOR MOUSE LOOK");

    guide.mouseLookEnabled = true;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: GATHER WOOD STONE FIBER");

    guide.wood = 1;
    guide.stone = 1;
    guide.fiber = 1;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: CRAFT PRIMITIVE_TOOL");

    guide.primitiveTool = 1;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: GATHER CRACKED ROCK");

    guide.crackedRockGathered = true;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: CRAFT CAMP_BUNDLE WORKBENCH_KIT");

    guide.campBundle = 1;
    guide.workbenchKit = 1;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: PLACE CAMP OR WORKBENCH");

    guide.placedCampMarker = true;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: DEFEAT REALM WISP");

    guide.realmWispDefeated = true;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: STABILISE FRACTURE");

    guide.fractureStabilised = true;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: CRAFT REALM_ANCHOR");

    guide.realmAnchor = 1;
    assert(rw::game::VerticalSliceGuide::NextStep(guide) == "NEXT: SAVE O / LOAD P");

    AssertShort(rw::game::ContextualPrompt::Build(prompt));
    AssertShort(rw::game::VerticalSliceGuide::NextStep(guide));
}
