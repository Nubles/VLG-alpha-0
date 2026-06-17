#pragma once

#include <string>

namespace rw::game {

class ObjectiveState {
public:
    void Complete(const std::string& objectiveId);
    bool IsCompleted(const std::string& objectiveId) const;
    void Reset();

private:
    std::string m_completedObjectiveId;
};

} // namespace rw::game
