#pragma once

#include <chrono>

namespace rw::core {

class Clock {
public:
    Clock();

    float Tick();
    float DeltaSeconds() const;
    double TotalSeconds() const;

private:
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint m_start;
    TimePoint m_previous;
    float m_deltaSeconds = 0.0F;
};

} // namespace rw::core

