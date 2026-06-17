#pragma once

#include "Game/Source/Building/BuildableDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

class BuildableDatabase {
public:
    static BuildableDatabase CreateStarterBuildables();

    void AddBuildable(BuildableDefinition buildable);
    const BuildableDefinition* FindById(const std::string& buildableId) const;
    const std::vector<BuildableDefinition>& Buildables() const;

private:
    std::vector<BuildableDefinition> m_buildables;
};

} // namespace rw::game
