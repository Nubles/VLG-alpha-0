#include "Game/Source/Building/BuildableDatabase.h"

#include <utility>

namespace rw::game {

BuildableDatabase BuildableDatabase::CreateStarterBuildables()
{
    BuildableDatabase database;

    database.AddBuildable({
        "camp_marker",
        "Camp Marker",
        { { "camp_bundle", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 0.8F, 1.0F, 0.8F },
        { 0.8F, 1.0F, 0.8F },
        3.0F,
        0.75F,
        "Camp",
    });

    database.AddBuildable({
        "workbench_stub",
        "Workbench Stub",
        { { "workbench_kit", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 1.8F, 0.8F, 0.9F },
        { 1.8F, 0.8F, 0.9F },
        3.0F,
        1.15F,
        "Stations",
    });

    database.AddBuildable({
        "simple_wall",
        "Simple Wall",
        { { "wood", 3 }, { "fiber", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 2.0F, 1.8F, 0.25F },
        { 2.0F, 1.8F, 0.25F },
        3.0F,
        1.05F,
        "Walls",
    });

    return database;
}

void BuildableDatabase::AddBuildable(BuildableDefinition buildable)
{
    m_buildables.push_back(std::move(buildable));
}

const BuildableDefinition* BuildableDatabase::FindById(const std::string& buildableId) const
{
    for (const BuildableDefinition& buildable : m_buildables) {
        if (buildable.id == buildableId) {
            return &buildable;
        }
    }

    return nullptr;
}

const std::vector<BuildableDefinition>& BuildableDatabase::Buildables() const
{
    return m_buildables;
}

} // namespace rw::game
