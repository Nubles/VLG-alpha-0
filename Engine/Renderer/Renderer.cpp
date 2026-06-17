#include "Engine/Renderer/Renderer.h"

namespace rw::renderer {

void Renderer::BeginFrame(rw::platform::Window& window)
{
    window.ClearBlankScreen();
}

void Renderer::EndFrame()
{
}

} // namespace rw::renderer

