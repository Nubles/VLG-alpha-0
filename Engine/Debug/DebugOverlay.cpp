#include "Engine/Debug/DebugOverlay.h"

#include <iomanip>
#include <sstream>

namespace rw::debug {

void DebugOverlay::Update(rw::platform::Window& window, float deltaSeconds)
{
    ++m_frameCount;
    m_timeUntilTitleRefresh -= deltaSeconds;

    if (m_timeUntilTitleRefresh > 0.0F) {
        return;
    }

    const float fps = deltaSeconds > 0.0F ? 1.0F / deltaSeconds : 0.0F;

    std::ostringstream title;
    title << "Realmbound Wilds - Engine Bootstrap | dt "
          << std::fixed << std::setprecision(3) << deltaSeconds
          << "s | fps " << std::setprecision(1) << fps
          << " | frame " << m_frameCount;

    window.SetTitle(title.str());
    m_timeUntilTitleRefresh = 0.25F;
}

} // namespace rw::debug

