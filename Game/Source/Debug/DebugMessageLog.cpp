#include "Game/Source/Debug/DebugMessageLog.h"

#include <algorithm>
#include <sstream>
#include <utility>

namespace rw::game {

DebugMessageLog::DebugMessageLog(int capacity)
    : m_capacity(std::max(1, capacity))
{
}

void DebugMessageLog::Push(std::string message)
{
    if (message.empty()) {
        return;
    }

    m_messages.push_back(std::move(message));
    while (static_cast<int>(m_messages.size()) > m_capacity) {
        m_messages.erase(m_messages.begin());
    }
}

const std::vector<std::string>& DebugMessageLog::Messages() const
{
    return m_messages;
}

std::string DebugMessageLog::Latest() const
{
    return m_messages.empty() ? std::string {} : m_messages.back();
}

std::string DebugMessageLog::Summary() const
{
    if (m_messages.empty()) {
        return "none";
    }

    std::ostringstream text;
    for (size_t index = 0; index < m_messages.size(); ++index) {
        if (index > 0) {
            text << " / ";
        }
        text << m_messages[index];
    }
    return text.str();
}

int DebugMessageLog::Capacity() const
{
    return m_capacity;
}

} // namespace rw::game
