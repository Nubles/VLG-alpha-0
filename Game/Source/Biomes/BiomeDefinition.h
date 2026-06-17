#pragma once

#include "Engine/Math/Vec3.h"

#include <string>
#include <vector>

namespace rw::game {

struct BiomeDefinition {
    std::string id;
    std::string displayName;
    std::string description;
    int difficultyTier = 0;
    rw::math::Vec3 debugClearColor { 0.06F, 0.075F, 0.10F };
    std::string ambienceText;
    std::vector<std::string> resourceThemeTags;
    std::vector<std::string> enemyThemeTags;
    std::vector<std::string> landmarkNames;
    std::string futureGuardianId;
    std::string futureWeatherProfileId;
};

} // namespace rw::game
