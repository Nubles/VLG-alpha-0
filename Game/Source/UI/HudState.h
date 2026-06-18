#pragma once

#include <string>
#include <vector>

namespace rw::game {

struct HudState {
    float health = 100.0F;
    float maxHealth = 100.0F;
    float stamina = 100.0F;
    float maxStamina = 100.0F;
    std::string currentTargetName = "None";
    std::string inventorySummary;
    std::string enemyName;
    float enemyHealth = 0.0F;
    float enemyMaxHealth = 1.0F;
    std::string enemyState;
    std::string objectiveStatus;
    std::string contextualPrompt;
    std::string nextStep;
    bool buildModeActive = false;
    bool mouseLookEnabled = false;
    std::string selectedBuildableName = "None";
    std::vector<std::string> messages;
    std::string saveLoadMessage;
};

} // namespace rw::game
