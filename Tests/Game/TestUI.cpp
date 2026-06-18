#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/UI/HudFormatter.h"
#include "Game/Source/UI/VerticalSliceHud.h"

#include <cassert>
#include <string>

void TestUI()
{
    rw::game::HudState state;
    state.health = 75.0F;
    state.maxHealth = 100.0F;
    state.stamina = 25.0F;
    state.maxStamina = 50.0F;
    state.currentTargetName = "";
    state.objectiveStatus = "fracture stable";
    state.buildModeActive = false;
    state.selectedBuildableName = "Camp Marker";
    state.messages = { "first", "second", "third" };

    assert(rw::game::HudFormatter::ClampedRatio(75.0F, 100.0F) == 0.75F);
    assert(rw::game::HudFormatter::ClampedRatio(150.0F, 100.0F) == 1.0F);
    assert(rw::game::HudFormatter::ClampedRatio(-10.0F, 100.0F) == 0.0F);
    assert(rw::game::HudFormatter::ClampedRatio(50.0F, 0.0F) == 0.0F);

    assert(rw::game::HudFormatter::TargetText(state) == "None");
    assert(rw::game::HudFormatter::BuildModeText(state) == "Build: Off");

    state.buildModeActive = true;
    assert(rw::game::HudFormatter::BuildModeText(state) == "Build: Camp Marker");
    assert(rw::game::HudFormatter::ObjectiveText(state) == "fracture stable");
    assert(state.messages[0] == "first");
    assert(state.messages[1] == "second");
    assert(state.messages[2] == "third");

    rw::game::VerticalSliceHud hud;
    const std::vector<rw::renderer::OverlayRect> rects = hud.BuildOverlay(state, 1280, 720);
    assert(!rects.empty());

    bool foundHealthFill = false;
    bool foundStaminaFill = false;
    bool foundBuildIndicator = false;
    for (const rw::renderer::OverlayRect& rect : rects) {
        if (rect.color.x > 0.7F && rect.color.y < 0.2F) {
            foundHealthFill = true;
        }
        if (rect.color.y > 0.6F && rect.color.z > 0.5F) {
            foundStaminaFill = true;
        }
        if (rect.color.y > 0.3F && rect.color.z < 0.3F && rect.y > 600.0F) {
            foundBuildIndicator = true;
        }
    }

    assert(foundHealthFill);
    assert(foundStaminaFill);
    assert(foundBuildIndicator);
}
