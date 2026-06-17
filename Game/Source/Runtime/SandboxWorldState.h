#pragma once

#include "Game/Source/Building/BuildPlacementResult.h"
#include "Game/Source/Resources/GatherableNode.h"

#include <string>
#include <vector>

namespace rw::game {

class SandboxWorldState {
public:
    std::string AllocatePlacedBuildableId();
    void RegisterPlacedBuildable(PlacedBuildable placed);
    void ReplacePlacedBuildables(std::vector<PlacedBuildable> placedBuildables);
    const std::vector<PlacedBuildable>& PlacedBuildables() const;
    std::vector<PlacedBuildable>& MutablePlacedBuildables();

    std::vector<std::string> DepletedGatherableIds(const std::vector<GatherableNode>& gatherables) const;
    int ApplyDepletedGatherableIds(
        std::vector<GatherableNode>& gatherables,
        const std::vector<std::string>& depletedIds) const;

private:
    int m_nextPlacedBuildableIndex = 1;
    std::vector<PlacedBuildable> m_placedBuildables;
};

} // namespace rw::game
