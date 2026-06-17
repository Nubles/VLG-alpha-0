#pragma once

#include "Engine/Input/InputState.h"
#include "Engine/Renderer/Renderer.h"

namespace rw::core {

class GameLayer {
public:
    virtual ~GameLayer() = default;
    virtual void OnUpdate(float deltaSeconds, const rw::input::InputState& input) = 0;
    virtual void OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window) = 0;
};

} // namespace rw::core

