#include "Game/Source/Biomes/BiomeDatabase.h"

#include <utility>

namespace rw::game {

BiomeDatabase BiomeDatabase::CreateStarterBiomes()
{
    BiomeDatabase database;
    database.AddBiome({
        "mistwood_hollow",
        "Mistwood Hollow",
        "A quiet, mist-heavy woodland where unstable realm energy leaks through old stones and broken roots.",
        1,
        { 0.06F, 0.085F, 0.08F },
        "Cold mist gathers between old roots and broken waystones.",
        { "wood", "stone", "fiber", "raw_food" },
        { "Realm Wisp" },
        { "Old Root Circle", "Broken Waystone", "Realm Fracture" },
        {},
        {},
    });
    return database;
}

void BiomeDatabase::AddBiome(BiomeDefinition biome)
{
    m_biomes.push_back(std::move(biome));
}

const BiomeDefinition* BiomeDatabase::FindById(const std::string& biomeId) const
{
    for (const BiomeDefinition& biome : m_biomes) {
        if (biome.id == biomeId) {
            return &biome;
        }
    }

    return nullptr;
}

const std::vector<BiomeDefinition>& BiomeDatabase::Biomes() const
{
    return m_biomes;
}

} // namespace rw::game
