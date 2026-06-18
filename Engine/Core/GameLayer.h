#pragma once

#include "Engine/Input/InputState.h"
#include "Engine/Renderer/Renderer.h"

#include <string>

namespace rw::core {

class GameLayer {
public:
    virtual ~GameLayer() = default;
    virtual void OnUpdate(float deltaSeconds, rw::input::InputState& input) = 0;
    virtual void OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window) = 0;
    virtual std::string DebugTitle() const;
};

} // namespace rw::core
