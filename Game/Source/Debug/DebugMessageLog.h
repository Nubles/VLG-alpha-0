#pragma once

#include <string>
#include <vector>

namespace rw::game {

class DebugMessageLog {
public:
    explicit DebugMessageLog(int capacity = 4);

    void Push(std::string message);
    const std::vector<std::string>& Messages() const;
    std::string Latest() const;
    std::string Summary() const;
    int Capacity() const;

private:
    int m_capacity = 4;
    std::vector<std::string> m_messages;
};

} // namespace rw::game
