#include "Engine/Input/InputState.h"
#include "Engine/Math/Mat4.h"
#include "Engine/Math/Vec3.h"
#include "Engine/Renderer/DebugTextRenderer.h"
#include "Game/Source/Interactable.h"
#include "Game/Source/PlayerVitals.h"

#include <cassert>
#include <cmath>

namespace {

bool NearlyEqual(float a, float b)
{
    return std::fabs(a - b) < 0.0001F;
}

} // namespace

int main()
{
    rw::input::InputState input;

    assert(!input.QuitRequested());
    assert(!input.EscapeDown());

    input.SetEscapeDown(true);
    assert(input.EscapeDown());
    assert(input.QuitRequested());

    input.BeginFrame();
    assert(!input.EscapeDown());
    assert(input.QuitRequested());

    input.SetKeyDown(rw::input::Key::E, true);
    assert(input.IsKeyDown(rw::input::Key::E));
    assert(input.WasKeyPressed(rw::input::Key::E));
    input.SetKeyDown(rw::input::Key::E, true);
    assert(!input.WasKeyPressed(rw::input::Key::E));
    input.SetKeyDown(rw::input::Key::M, true);
    assert(input.WasKeyPressed(rw::input::Key::M));

    input.SetMousePosition(100, 200);
    assert(input.MouseX() == 100);
    assert(input.MouseY() == 200);
    input.AddMouseDelta(5, -3);
    input.AddMouseDelta(2, 4);
    assert(input.MouseDeltaX() == 7);
    assert(input.MouseDeltaY() == 1);
    input.SetMouseCaptureEnabled(true);
    assert(input.MouseCaptureEnabled());
    input.BeginFrame();
    assert(input.MouseDeltaX() == 0);
    assert(input.MouseDeltaY() == 0);
    assert(input.MouseCaptureEnabled());

    const rw::math::Vec3 a { 1.0F, 2.0F, 3.0F };
    const rw::math::Vec3 b { 2.0F, 0.0F, 1.0F };
    const rw::math::Vec3 sum = a + b;
    assert(NearlyEqual(sum.x, 3.0F));
    assert(NearlyEqual(sum.y, 2.0F));
    assert(NearlyEqual(sum.z, 4.0F));

    const rw::math::Vec3 cross = rw::math::Cross({ 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    assert(NearlyEqual(cross.x, 0.0F));
    assert(NearlyEqual(cross.y, 0.0F));
    assert(NearlyEqual(cross.z, 1.0F));

    const rw::math::Mat4 identity = rw::math::Mat4::Identity();
    assert(NearlyEqual(identity.m[0], 1.0F));
    assert(NearlyEqual(identity.m[5], 1.0F));
    assert(NearlyEqual(identity.m[10], 1.0F));
    assert(NearlyEqual(identity.m[15], 1.0F));

    const rw::math::Mat4 view = rw::math::Mat4::LookAt(
        { 0.0F, 0.0F, 5.0F },
        { 0.0F, 0.0F, 0.0F },
        { 0.0F, 1.0F, 0.0F });
    assert(NearlyEqual(view.m[14], -5.0F));

    rw::game::PlayerVitals vitals;
    vitals.Update(1.0F, true, true);
    assert(vitals.Stamina() < 100.0F);
    assert(vitals.IsSprinting());

    const float drainedStamina = vitals.Stamina();
    vitals.Update(0.25F, false, false);
    assert(NearlyEqual(vitals.Stamina(), drainedStamina));
    vitals.Update(1.0F, false, false);
    assert(vitals.Stamina() > drainedStamina);

    rw::game::Interactable interactable;
    interactable.name = "Ancient Stone";
    interactable.transform.position = { 0.0F, 0.0F, -4.0F };
    interactable.interactionRadius = 0.75F;
    assert(interactable.HitBy({ { 0.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, -1.0F }, 6.0F }));
    assert(!interactable.HitBy({ { 0.0F, 0.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, 6.0F }));

    assert(rw::renderer::DebugTextRenderer::HasGlyph('A'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph('Z'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph('0'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph('9'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph(':'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph('-'));
    assert(rw::renderer::DebugTextRenderer::HasGlyph('_'));
    assert(!rw::renderer::DebugTextRenderer::HasGlyph('@'));

    assert(rw::renderer::DebugTextRenderer::Sanitize("hp: 10/20!") == "HP: 10/20!");
    assert(rw::renderer::DebugTextRenderer::Sanitize("bad@text\nok") == "BAD?TEXT OK");
    assert(rw::renderer::DebugTextRenderer::Sanitize("abcdef", 3) == "ABC");

    rw::renderer::OverlayText emptyText;
    assert(rw::renderer::DebugTextRenderer::BuildTextRects(emptyText).empty());

    rw::renderer::OverlayText text;
    text.text = "A0:-_";
    text.x = 4.0F;
    text.y = 8.0F;
    text.scale = 2.0F;
    assert(!rw::renderer::DebugTextRenderer::BuildTextRects(text).empty());

    return 0;
}
