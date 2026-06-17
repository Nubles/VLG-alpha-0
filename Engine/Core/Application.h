#pragma once

#include "Engine/Core/Clock.h"
#include "Engine/Debug/DebugOverlay.h"
#include "Engine/Input/InputState.h"
#include "Engine/Platform/Window.h"
#include "Engine/Renderer/Renderer.h"

namespace rw::core {

class Application {
public:
    int Run();

private:
    Clock m_clock;
    rw::input::InputState m_input;
    rw::renderer::Renderer m_renderer;
    rw::debug::DebugOverlay m_debugOverlay;
};

} // namespace rw::core

