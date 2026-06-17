#pragma once

#include "Engine/Core/GameLayer.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Scene.h"

namespace rw::game {

class SandboxGame final : public rw::core::GameLayer {
public:
    SandboxGame();

    void OnUpdate(float deltaSeconds, const rw::input::InputState& input) override;
    void OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window) override;

private:
    rw::scene::Scene m_scene;
    rw::scene::Camera m_camera;
};

} // namespace rw::game

