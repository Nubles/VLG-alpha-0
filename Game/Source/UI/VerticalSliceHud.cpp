#include "Game/Source/UI/VerticalSliceHud.h"

#include "Game/Source/UI/HudFormatter.h"

#include <algorithm>

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

void VerticalSliceHud::Render(rw::renderer::Renderer& renderer, rw::platform::Window& window, const HudState& state) const
{
    renderer.RenderOverlay(window, BuildOverlay(state, window.Width(), window.Height()));
}

} // namespace rw::game
