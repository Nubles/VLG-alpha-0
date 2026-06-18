#include "Engine/Renderer/DebugTextRenderer.h"

#include <array>
#include <cctype>

namespace rw::renderer {

namespace {

using GlyphRows = std::array<unsigned char, 7>;

constexpr GlyphRows Blank()
{
    return { 0, 0, 0, 0, 0, 0, 0 };
}

GlyphRows GlyphFor(char character)
{
    switch (character) {
    case 'A': return { 14, 17, 17, 31, 17, 17, 17 };
    case 'B': return { 30, 17, 17, 30, 17, 17, 30 };
    case 'C': return { 14, 17, 16, 16, 16, 17, 14 };
    case 'D': return { 30, 17, 17, 17, 17, 17, 30 };
    case 'E': return { 31, 16, 16, 30, 16, 16, 31 };
    case 'F': return { 31, 16, 16, 30, 16, 16, 16 };
    case 'G': return { 14, 17, 16, 23, 17, 17, 15 };
    case 'H': return { 17, 17, 17, 31, 17, 17, 17 };
    case 'I': return { 14, 4, 4, 4, 4, 4, 14 };
    case 'J': return { 7, 2, 2, 2, 18, 18, 12 };
    case 'K': return { 17, 18, 20, 24, 20, 18, 17 };
    case 'L': return { 16, 16, 16, 16, 16, 16, 31 };
    case 'M': return { 17, 27, 21, 21, 17, 17, 17 };
    case 'N': return { 17, 25, 21, 19, 17, 17, 17 };
    case 'O': return { 14, 17, 17, 17, 17, 17, 14 };
    case 'P': return { 30, 17, 17, 30, 16, 16, 16 };
    case 'Q': return { 14, 17, 17, 17, 21, 18, 13 };
    case 'R': return { 30, 17, 17, 30, 20, 18, 17 };
    case 'S': return { 15, 16, 16, 14, 1, 1, 30 };
    case 'T': return { 31, 4, 4, 4, 4, 4, 4 };
    case 'U': return { 17, 17, 17, 17, 17, 17, 14 };
    case 'V': return { 17, 17, 17, 17, 17, 10, 4 };
    case 'W': return { 17, 17, 17, 21, 21, 21, 10 };
    case 'X': return { 17, 17, 10, 4, 10, 17, 17 };
    case 'Y': return { 17, 17, 10, 4, 4, 4, 4 };
    case 'Z': return { 31, 1, 2, 4, 8, 16, 31 };
    case '0': return { 14, 17, 19, 21, 25, 17, 14 };
    case '1': return { 4, 12, 4, 4, 4, 4, 14 };
    case '2': return { 14, 17, 1, 2, 4, 8, 31 };
    case '3': return { 30, 1, 1, 14, 1, 1, 30 };
    case '4': return { 2, 6, 10, 18, 31, 2, 2 };
    case '5': return { 31, 16, 16, 30, 1, 1, 30 };
    case '6': return { 14, 16, 16, 30, 17, 17, 14 };
    case '7': return { 31, 1, 2, 4, 8, 8, 8 };
    case '8': return { 14, 17, 17, 14, 17, 17, 14 };
    case '9': return { 14, 17, 17, 15, 1, 1, 14 };
    case ':': return { 0, 4, 4, 0, 4, 4, 0 };
    case '/': return { 1, 1, 2, 4, 8, 16, 16 };
    case '-': return { 0, 0, 0, 14, 0, 0, 0 };
    case '_': return { 0, 0, 0, 0, 0, 0, 31 };
    case '.': return { 0, 0, 0, 0, 0, 12, 12 };
    case ',': return { 0, 0, 0, 0, 0, 4, 8 };
    case '!': return { 4, 4, 4, 4, 4, 0, 4 };
    case '?': return { 14, 17, 1, 2, 4, 0, 4 };
    case '(': return { 2, 4, 8, 8, 8, 4, 2 };
    case ')': return { 8, 4, 2, 2, 2, 4, 8 };
    case ' ': return Blank();
    default: return Blank();
    }
}

bool IsSupported(char character)
{
    return (character >= 'A' && character <= 'Z')
        || (character >= '0' && character <= '9')
        || character == ' '
        || character == ':'
        || character == '/'
        || character == '-'
        || character == '_'
        || character == '.'
        || character == ','
        || character == '!'
        || character == '?'
        || character == '('
        || character == ')';
}

OverlayRect PixelRect(float x, float y, float scale, rw::math::Vec3 color, float alpha)
{
    OverlayRect rect;
    rect.x = x;
    rect.y = y;
    rect.width = scale;
    rect.height = scale;
    rect.color = color;
    rect.alpha = alpha;
    return rect;
}

} // namespace

bool DebugTextRenderer::HasGlyph(char character)
{
    if (character == '\n' || character == '\r' || character == '\t') {
        return false;
    }

    const unsigned char value = static_cast<unsigned char>(character);
    const char normalized = static_cast<char>(std::toupper(value));
    return IsSupported(normalized);
}

std::string DebugTextRenderer::Sanitize(std::string_view text, std::size_t maxCharacters)
{
    std::string sanitized;
    sanitized.reserve(maxCharacters > 0 ? maxCharacters : text.size());

    for (char character : text) {
        if (maxCharacters > 0 && sanitized.size() >= maxCharacters) {
            break;
        }
        if (character == '\n' || character == '\r' || character == '\t') {
            sanitized.push_back(' ');
            continue;
        }

        unsigned char value = static_cast<unsigned char>(character);
        char normalized = static_cast<char>(std::toupper(value));
        sanitized.push_back(IsSupported(normalized) ? normalized : '?');
    }

    return sanitized;
}

std::vector<OverlayRect> DebugTextRenderer::BuildTextRects(const OverlayText& text)
{
    const std::string sanitized = Sanitize(text.text, text.maxCharacters);
    if (sanitized.empty() || text.scale <= 0.0F || text.alpha <= 0.0F) {
        return {};
    }

    std::vector<OverlayRect> rects;
    rects.reserve(sanitized.size() * 12);

    constexpr int glyphWidth = 5;
    constexpr int glyphHeight = 7;
    constexpr int glyphAdvance = 6;

    for (std::size_t index = 0; index < sanitized.size(); ++index) {
        const GlyphRows glyph = GlyphFor(sanitized[index]);
        const float glyphX = text.x + static_cast<float>(index * glyphAdvance) * text.scale;

        for (int row = 0; row < glyphHeight; ++row) {
            for (int column = 0; column < glyphWidth; ++column) {
                const unsigned char mask = static_cast<unsigned char>(1U << (glyphWidth - column - 1));
                if ((glyph[static_cast<std::size_t>(row)] & mask) == 0) {
                    continue;
                }

                rects.push_back(PixelRect(
                    glyphX + static_cast<float>(column) * text.scale,
                    text.y + static_cast<float>(row) * text.scale,
                    text.scale,
                    text.color,
                    text.alpha));
            }
        }
    }

    return rects;
}

} // namespace rw::renderer
