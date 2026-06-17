#pragma once

#include <string>
#include <vector>

namespace rw::game {

class ProgressionState {
public:
    void SetFlag(const std::string& flag);
    bool HasFlag(const std::string& flag) const;
    void Clear();
    std::string Summary() const;
    const std::vector<std::string>& Flags() const;

private:
    std::vector<std::string> m_flags;
};

} // namespace rw::game
