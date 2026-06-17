#include "Game/Source/Progression/ProgressionState.h"

#include <sstream>

namespace rw::game {

void ProgressionState::SetFlag(const std::string& flag)
{
    if (flag.empty() || HasFlag(flag)) {
        return;
    }

    m_flags.push_back(flag);
}

bool ProgressionState::HasFlag(const std::string& flag) const
{
    for (const std::string& activeFlag : m_flags) {
        if (activeFlag == flag) {
            return true;
        }
    }

    return false;
}

void ProgressionState::Clear()
{
    m_flags.clear();
}

std::string ProgressionState::Summary() const
{
    if (m_flags.empty()) {
        return "none";
    }

    std::ostringstream summary;
    for (size_t index = 0; index < m_flags.size(); ++index) {
        if (index > 0) {
            summary << ",";
        }
        summary << m_flags[index];
    }
    return summary.str();
}

const std::vector<std::string>& ProgressionState::Flags() const
{
    return m_flags;
}

} // namespace rw::game
