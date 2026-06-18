#pragma once

#include "Game/Source/Building/BuildableDefinition.h"

#include <string>
#include <vector>

namespace rw::game {

struct BuildableDatabaseLoadResult;

class BuildableDatabase {
public:
    static BuildableDatabase CreateStarterBuildables();
    static BuildableDatabaseLoadResult LoadFromText(const std::string& text);
    static BuildableDatabaseLoadResult LoadFromFile(const std::string& path);
    static BuildableDatabase CreateFromFileOrFallback(const std::string& path);

    void AddBuildable(BuildableDefinition buildable);
    const BuildableDefinition* FindById(const std::string& buildableId) const;
    const std::vector<BuildableDefinition>& Buildables() const;

private:
    std::vector<BuildableDefinition> m_buildables;
};

struct BuildableDatabaseLoadResult {
    bool success = false;
    std::string message;
    BuildableDatabase database;
};

} // namespace rw::game
