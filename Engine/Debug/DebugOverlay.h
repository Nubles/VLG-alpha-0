#pragma once

#include "Engine/Platform/Window.h"

namespace rw::debug {

class DebugOverlay {
public:
    void Update(rw::platform::Window& window, float deltaSeconds);

private:
    float m_timeUntilTitleRefresh = 0.0F;
    int m_frameCount = 0;
};

} // namespace rw::debug

