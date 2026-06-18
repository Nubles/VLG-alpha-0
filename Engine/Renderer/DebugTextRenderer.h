#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Renderer/Renderer.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace rw::renderer {

struct OverlayText {
    std::string text;
    float x = 0.0F;
    float y = 0.0F;
    float scale = 2.0F;
    rw::math::Vec3 color { 1.0F, 1.0F, 1.0F };
    float alpha = 1.0F;
    std::size_t maxCharacters = 0;
};

class DebugTextRenderer {
public:
    static bool HasGlyph(char character);
    static std::string Sanitize(std::string_view text, std::size_t maxCharacters = 0);
    static std::vector<OverlayRect> BuildTextRects(const OverlayText& text);
};

} // namespace rw::renderer
