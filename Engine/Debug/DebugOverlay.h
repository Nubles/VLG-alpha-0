#pragma once

#include "Engine/Platform/Window.h"

#include <string>

namespace rw::debug {

class DebugOverlay {
public:
    void Update(rw::platform::Window& window, float deltaSeconds, const std::string& gameDebugText);

private:
    float m_timeUntilTitleRefresh = 0.0F;
    int m_frameCount = 0;
};

} // namespace rw::debug
