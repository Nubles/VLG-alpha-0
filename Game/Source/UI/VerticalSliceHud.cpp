#include "Game/Source/UI/VerticalSliceHud.h"

#include "Engine/Renderer/DebugTextRenderer.h"
#include "Game/Source/UI/HudFormatter.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>

namespace rw::game {

namespace {

rw::renderer::OverlayRect Rect(float x, float y, float width, float height, rw::math::Vec3 color, float alpha)
{
    rw::renderer::OverlayRect rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    rect.color = color;
    rect.alpha = alpha;
    return rect;
}

void AppendText(
    std::vector<rw::renderer::OverlayRect>& rects,
    std::string text,
    float x,
    float y,
    float scale,
    rw::math::Vec3 color,
    float alpha,
    std::size_t maxCharacters = 0)
{
    rw::renderer::OverlayText overlayText;
    overlayText.text = std::move(text);
    overlayText.x = x;
    overlayText.y = y;
    overlayText.scale = scale;
    overlayText.color = color;
    overlayText.alpha = alpha;
    overlayText.maxCharacters = maxCharacters;

    std::vector<rw::renderer::OverlayRect> textRects = rw::renderer::DebugTextRenderer::BuildTextRects(overlayText);
    rects.insert(rects.end(), textRects.begin(), textRects.end());
}

std::string WholeNumber(float value)
{
    std::ostringstream text;
    text << std::fixed << std::setprecision(0) << value;
    return text.str();
}

} // namespace

std::vector<rw::renderer::OverlayRect> VerticalSliceHud::BuildOverlay(
    const HudState& state, int windowWidth, int windowHeight) const
{
    const float width = static_cast<float>(std::max(windowWidth, 1));
    const float height = static_cast<float>(std::max(windowHeight, 1));
    const float margin = 18.0F;
    const float barWidth = std::min(260.0F, width * 0.32F);
    const float barHeight = 14.0F;
    const float panelWidth = std::min(320.0F, width * 0.34F);
    const float panelHeight = 28.0F;
    const float healthRatio = HudFormatter::ClampedRatio(state.health, state.maxHealth);
    const float staminaRatio = HudFormatter::ClampedRatio(state.stamina, state.maxStamina);
    const float enemyRatio = HudFormatter::ClampedRatio(state.enemyHealth, state.enemyMaxHealth);

    std::vector<rw::renderer::OverlayRect> rects;
    rects.reserve(12);

    rects.push_back(Rect(margin - 4.0F, height - 70.0F, barWidth + 8.0F, 48.0F, { 0.02F, 0.025F, 0.03F }, 0.72F));
    rects.push_back(Rect(margin, height - 62.0F, barWidth, barHeight, { 0.16F, 0.04F, 0.04F }, 0.90F));
    rects.push_back(Rect(margin, height - 62.0F, barWidth * healthRatio, barHeight, { 0.80F, 0.12F, 0.10F }, 0.95F));
    rects.push_back(Rect(margin, height - 38.0F, barWidth, barHeight, { 0.05F, 0.11F, 0.12F }, 0.90F));
    rects.push_back(Rect(margin, height - 38.0F, barWidth * staminaRatio, barHeight, { 0.10F, 0.70F, 0.55F }, 0.95F));

    const float topY = margin;
    rects.push_back(Rect(margin, topY, panelWidth, panelHeight, { 0.03F, 0.04F, 0.05F }, 0.62F));
    if (HudFormatter::TargetText(state) != "None") {
        rects.push_back(Rect(margin + 6.0F, topY + 6.0F, panelWidth - 12.0F, panelHeight - 12.0F, { 0.35F, 0.62F, 0.95F }, 0.78F));
    }

    const float objectiveY = topY + panelHeight + 8.0F;
    const rw::math::Vec3 objectiveColor = HudFormatter::ObjectiveText(state).find("stable") != std::string::npos
        ? rw::math::Vec3 { 0.48F, 0.78F, 0.85F }
        : rw::math::Vec3 { 0.45F, 0.38F, 0.82F };
    rects.push_back(Rect(margin, objectiveY, panelWidth, panelHeight, { 0.03F, 0.04F, 0.05F }, 0.62F));
    rects.push_back(Rect(margin + 6.0F, objectiveY + 6.0F, panelWidth - 12.0F, panelHeight - 12.0F, objectiveColor, 0.64F));

    if (state.enemyHealth > 0.0F) {
        const float enemyWidth = std::min(220.0F, width * 0.28F);
        rects.push_back(Rect(width - enemyWidth - margin, margin, enemyWidth, 12.0F, { 0.13F, 0.03F, 0.13F }, 0.86F));
        rects.push_back(Rect(width - enemyWidth - margin, margin, enemyWidth * enemyRatio, 12.0F, { 0.72F, 0.20F, 0.88F }, 0.92F));
    }

    if (state.buildModeActive) {
        rects.push_back(Rect(width - panelWidth - margin, height - 56.0F, panelWidth, 28.0F, { 0.10F, 0.35F, 0.24F }, 0.76F));
    }

    if (!state.messages.empty() || !state.saveLoadMessage.empty()) {
        rects.push_back(Rect(margin, objectiveY + panelHeight + 8.0F, panelWidth, 24.0F, { 0.04F, 0.05F, 0.055F }, 0.58F));
    }

    return rects;
}

std::vector<std::string> VerticalSliceHud::BuildTextLines(const HudState& state) const
{
    std::vector<std::string> lines;
    lines.push_back("HP " + WholeNumber(state.health) + "/" + WholeNumber(state.maxHealth));
    lines.push_back("ST " + WholeNumber(state.stamina) + "/" + WholeNumber(state.maxStamina));
    lines.push_back("TARGET: " + HudFormatter::TargetText(state));
    lines.push_back("OBJECTIVE: " + HudFormatter::ObjectiveText(state));

    if (state.enemyHealth > 0.0F) {
        lines.push_back(state.enemyName + " " + WholeNumber(state.enemyHealth) + "/"
            + WholeNumber(state.enemyMaxHealth) + " " + state.enemyState);
    }

    if (state.buildModeActive) {
        lines.push_back(HudFormatter::BuildModeText(state));
    }

    const int messageCount = static_cast<int>(state.messages.size());
    const int firstMessage = std::max(0, messageCount - 3);
    for (int index = firstMessage; index < messageCount; ++index) {
        lines.push_back(state.messages[static_cast<std::size_t>(index)]);
    }

    if (!state.saveLoadMessage.empty()) {
        lines.push_back("SAVE: " + state.saveLoadMessage);
    }

    return lines;
}

std::vector<rw::renderer::OverlayRect> VerticalSliceHud::BuildTextOverlay(
    const HudState& state, int windowWidth, int windowHeight) const
{
    const float width = static_cast<float>(std::max(windowWidth, 1));
    const float height = static_cast<float>(std::max(windowHeight, 1));
    const float margin = 18.0F;
    const float panelWidth = std::min(320.0F, width * 0.34F);
    const float panelHeight = 28.0F;
    const rw::math::Vec3 softWhite { 0.88F, 0.94F, 0.90F };
    const rw::math::Vec3 mutedWhite { 0.66F, 0.76F, 0.74F };

    std::vector<rw::renderer::OverlayRect> rects;
    rects.reserve(320);

    const std::vector<std::string> lines = BuildTextLines(state);

    AppendText(rects, lines[0],
        margin, height - 86.0F, 2.0F, softWhite, 0.95F, 18);
    AppendText(rects, lines[1],
        margin, height - 18.0F, 2.0F, softWhite, 0.95F, 18);

    AppendText(rects, lines[2],
        margin + 8.0F, margin + 8.0F, 2.0F, softWhite, 0.95F, 36);
    AppendText(rects, lines[3],
        margin + 8.0F, margin + panelHeight + 16.0F, 2.0F, softWhite, 0.95F, 38);

    std::size_t nextLine = 4;
    if (state.enemyHealth > 0.0F) {
        AppendText(rects,
            lines[nextLine],
            width - std::min(220.0F, width * 0.28F),
            margin + 18.0F,
            2.0F,
            softWhite,
            0.95F,
            28);
        ++nextLine;
    }

    if (state.buildModeActive) {
        AppendText(rects, lines[nextLine],
            width - panelWidth - margin + 8.0F, height - 48.0F, 2.0F, softWhite, 0.95F, 32);
        ++nextLine;
    }

    const float messageY = margin + (panelHeight * 2.0F) + 24.0F;
    const int messageCount = static_cast<int>(state.messages.size());
    const int firstMessage = std::max(0, messageCount - 3);
    for (int index = firstMessage; index < messageCount; ++index) {
        AppendText(rects,
            lines[nextLine],
            margin + 8.0F,
            messageY + static_cast<float>(index - firstMessage) * 18.0F,
            2.0F,
            mutedWhite,
            0.90F,
            44);
        ++nextLine;
    }

    if (!state.saveLoadMessage.empty()) {
        AppendText(rects, lines[nextLine],
            margin + 8.0F,
            messageY + static_cast<float>(std::min(messageCount, 3)) * 18.0F,
            2.0F,
            softWhite,
            0.92F,
            44);
    }

    return rects;
}

void VerticalSliceHud::Render(rw::renderer::Renderer& renderer, rw::platform::Window& window, const HudState& state) const
{
    std::vector<rw::renderer::OverlayRect> rects = BuildOverlay(state, window.Width(), window.Height());
    std::vector<rw::renderer::OverlayRect> textRects = BuildTextOverlay(state, window.Width(), window.Height());
    rects.insert(rects.end(), textRects.begin(), textRects.end());
    renderer.RenderOverlay(window, rects);
}

} // namespace rw::game
