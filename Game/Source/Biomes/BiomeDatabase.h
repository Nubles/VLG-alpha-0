#pragma once

#include "Game/Source/Biomes/BiomeDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

class BiomeDatabase {
public:
    static BiomeDatabase CreateStarterBiomes();

    void AddBiome(BiomeDefinition biome);
    const BiomeDefinition* FindById(const std::string& biomeId) const;
    const std::vector<BiomeDefinition>& Biomes() const;

private:
    std::vector<BiomeDefinition> m_biomes;
};

} // namespace rw::game
