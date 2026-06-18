#pragma once

#include "Engine/Platform/Window.h"
#include "Engine/Math/Vec3.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Scene.h"

#include <vector>

namespace rw::renderer {

struct OverlayRect {
    float x = 0.0F;
    float y = 0.0F;
    float width = 0.0F;
    float height = 0.0F;
    rw::math::Vec3 color { 1.0F, 1.0F, 1.0F };
    float alpha = 1.0F;
};

class Renderer {
public:
    ~Renderer();

    void Initialize(rw::platform::Window& window);
    void Shutdown();
    void Render(rw::platform::Window& window, const rw::scene::Scene& scene, const rw::scene::Camera& camera);
    void RenderOverlay(rw::platform::Window& window, const std::vector<OverlayRect>& rects);
    void Present(rw::platform::Window& window);
    void BeginFrame(rw::platform::Window& window);
    void EndFrame();

private:
    void* m_renderContext = nullptr;
    void* m_deviceContext = nullptr;
};

} // namespace rw::renderer
