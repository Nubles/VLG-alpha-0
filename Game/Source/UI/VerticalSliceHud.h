#pragma once

#include "Engine/Renderer/Renderer.h"
#include "Game/Source/UI/HudState.h"

#include <string>
#include <vector>

namespace rw::game {

class VerticalSliceHud {
public:
    std::vector<rw::renderer::OverlayRect> BuildOverlay(const HudState& state, int windowWidth, int windowHeight) const;
    std::vector<std::string> BuildTextLines(const HudState& state) const;
    std::vector<rw::renderer::OverlayRect> BuildTextOverlay(
        const HudState& state, int windowWidth, int windowHeight) const;
    void Render(rw::renderer::Renderer& renderer, rw::platform::Window& window, const HudState& state) const;
};

} // namespace rw::game
