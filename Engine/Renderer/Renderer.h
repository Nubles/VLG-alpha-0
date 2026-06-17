#pragma once

#include "Engine/Platform/Window.h"

namespace rw::renderer {

class Renderer {
public:
    void BeginFrame(rw::platform::Window& window);
    void EndFrame();
};

} // namespace rw::renderer

