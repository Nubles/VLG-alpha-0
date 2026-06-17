#include "Engine/Core/Clock.h"

namespace rw::core {

Clock::Clock()
    : m_start(std::chrono::steady_clock::now())
    , m_previous(m_start)
{
}

float Clock::Tick()
{
    const TimePoint current = std::chrono::steady_clock::now();
    m_deltaSeconds = std::chrono::duration<float>(current - m_previous).count();
    m_previous = current;
    return m_deltaSeconds;
}

float Clock::DeltaSeconds() const
{
    return m_deltaSeconds;
}

double Clock::TotalSeconds() const
{
    return std::chrono::duration<double>(
        std::chrono::steady_clock::now() - m_start).count();
}

} // namespace rw::core

