#include "Engine/Input/InputState.h"
#include "Engine/Math/Mat4.h"
#include "Engine/Math/Vec3.h"
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

    return 0;
}
