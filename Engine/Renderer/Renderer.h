#pragma once

#include "Engine/Platform/Window.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Scene.h"

namespace rw::renderer {

class Renderer {
public:
    ~Renderer();

    void Initialize(rw::platform::Window& window);
    void Shutdown();
    void Render(rw::platform::Window& window, const rw::scene::Scene& scene, const rw::scene::Camera& camera);
    void BeginFrame(rw::platform::Window& window);
    void EndFrame();

private:
    void* m_renderContext = nullptr;
    void* m_deviceContext = nullptr;
};

} // namespace rw::renderer
