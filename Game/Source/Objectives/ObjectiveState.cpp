#include "Game/Source/Objectives/ObjectiveState.h"

namespace rw::game {

void ObjectiveState::Complete(const std::string& objectiveId)
{
    m_completedObjectiveId = objectiveId;
}

bool ObjectiveState::IsCompleted(const std::string& objectiveId) const
{
    return !objectiveId.empty() && m_completedObjectiveId == objectiveId;
}

void ObjectiveState::Reset()
{
    m_completedObjectiveId.clear();
}

} // namespace rw::game
