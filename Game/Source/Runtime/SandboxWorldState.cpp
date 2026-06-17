#include "Game/Source/Runtime/SandboxWorldState.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>

namespace rw::game {

std::string SandboxWorldState::AllocatePlacedBuildableId()
{
    std::ostringstream id;
    id << "placed_" << std::setw(4) << std::setfill('0') << m_nextPlacedBuildableIndex;
    ++m_nextPlacedBuildableIndex;
    return id.str();
}

void SandboxWorldState::RegisterPlacedBuildable(PlacedBuildable placed)
{
    if (placed.instanceId.empty()) {
        placed.instanceId = AllocatePlacedBuildableId();
    }
    m_placedBuildables.push_back(std::move(placed));
}

void SandboxWorldState::ReplacePlacedBuildables(std::vector<PlacedBuildable> placedBuildables)
{
    m_placedBuildables.clear();
    m_nextPlacedBuildableIndex = 1;

    for (PlacedBuildable& placed : placedBuildables) {
        if (placed.instanceId.empty()) {
            placed.instanceId = AllocatePlacedBuildableId();
        }
        m_placedBuildables.push_back(std::move(placed));
    }

    for (const PlacedBuildable& placed : m_placedBuildables) {
        constexpr const char* kPrefix = "placed_";
        if (placed.instanceId.rfind(kPrefix, 0) != 0) {
            continue;
        }

        try {
            const int parsed = std::stoi(placed.instanceId.substr(7));
            m_nextPlacedBuildableIndex = std::max(m_nextPlacedBuildableIndex, parsed + 1);
        } catch (...) {
        }
    }
}

const std::vector<PlacedBuildable>& SandboxWorldState::PlacedBuildables() const
{
    return m_placedBuildables;
}

std::vector<PlacedBuildable>& SandboxWorldState::MutablePlacedBuildables()
{
    return m_placedBuildables;
}

std::vector<std::string> SandboxWorldState::DepletedGatherableIds(
    const std::vector<GatherableNode>& gatherables) const
{
    std::vector<std::string> depletedIds;
    for (const GatherableNode& gatherable : gatherables) {
        if (!gatherable.id.empty() && gatherable.IsDepleted()) {
            depletedIds.push_back(gatherable.id);
        }
    }
    return depletedIds;
}

int SandboxWorldState::ApplyDepletedGatherableIds(
    std::vector<GatherableNode>& gatherables,
    const std::vector<std::string>& depletedIds) const
{
    int appliedCount = 0;
    for (const std::string& depletedId : depletedIds) {
        auto found = std::find_if(gatherables.begin(), gatherables.end(),
            [&depletedId](const GatherableNode& gatherable) {
                return gatherable.id == depletedId;
            });

        if (found == gatherables.end()) {
            continue;
        }

        found->remainingGatherCount = 0;
        ++appliedCount;
    }
    return appliedCount;
}

} // namespace rw::game
