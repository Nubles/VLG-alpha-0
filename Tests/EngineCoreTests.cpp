#include "Engine/Input/InputState.h"
#include "Engine/Math/Mat4.h"
#include "Engine/Math/Vec3.h"

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

    return 0;
}
