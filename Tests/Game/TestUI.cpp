#include "Tests/Game/GameTestSuite.h"

#include "Engine/Renderer/DebugTextRenderer.h"
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
    state.mouseLookEnabled = true;
    state.buildModeActive = false;
    state.selectedBuildableName = "Camp Marker";
    state.enemyName = "Realm Wisp";
    state.enemyHealth = 35.0F;
    state.enemyMaxHealth = 50.0F;
    state.enemyState = "Chasing";
    state.messages = { "first", "second", "third" };
    state.saveLoadMessage = "Saved game";

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
    const std::vector<rw::renderer::OverlayRect> textRects = hud.BuildTextOverlay(state, 1280, 720);
    assert(!textRects.empty());
    const std::vector<std::string> textLines = hud.BuildTextLines(state);
    assert(textLines[0] == "HP 75/100");
    assert(textLines[1] == "ST 25/50");
    assert(textLines[2] == "MOUSE: ON");
    assert(textLines[3] == "TARGET: None");
    assert(textLines[4] == "OBJECTIVE: fracture stable");
    assert(textLines[5] == "Realm Wisp 35/50 Chasing");
    assert(textLines[6] == "Build: Camp Marker");
    assert(textLines[7] == "first");
    assert(textLines[8] == "second");
    assert(textLines[9] == "third");
    assert(textLines[10] == "SAVE: Saved game");

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

    const std::string healthText = rw::renderer::DebugTextRenderer::Sanitize("HP 75/100");
    const std::string targetText = rw::renderer::DebugTextRenderer::Sanitize("TARGET: None");
    const std::string objectiveText = rw::renderer::DebugTextRenderer::Sanitize("OBJECTIVE: fracture stable");
    assert(healthText == "HP 75/100");
    assert(targetText == "TARGET: NONE");
    assert(objectiveText == "OBJECTIVE: FRACTURE STABLE");
}
