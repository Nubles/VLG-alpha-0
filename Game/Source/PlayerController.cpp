#include "Game/Source/PlayerController.h"

#include <algorithm>

namespace rw::game {

PlayerController::PlayerController()
{
    m_camera.position = { 0.0F, 1.8F, 6.0F };
    m_camera.yawRadians = 3.14159265F;
    m_camera.pitchRadians = -0.18F;
}

void PlayerController::Update(float deltaSeconds, const rw::input::InputState& input)
{
    constexpr float walkSpeed = 3.0F;
    constexpr float sprintSpeed = 6.5F;
    constexpr float lookSpeed = 1.8F;

    if (input.IsKeyDown(rw::input::Key::Left)) {
        m_camera.yawRadians += lookSpeed * deltaSeconds;
    }
    if (input.IsKeyDown(rw::input::Key::Right)) {
        m_camera.yawRadians -= lookSpeed * deltaSeconds;
    }
    if (input.IsKeyDown(rw::input::Key::Up)) {
        m_camera.pitchRadians += lookSpeed * deltaSeconds;
    }
    if (input.IsKeyDown(rw::input::Key::Down)) {
        m_camera.pitchRadians -= lookSpeed * deltaSeconds;
    }

    m_camera.pitchRadians = std::clamp(m_camera.pitchRadians, -1.45F, 1.45F);

    const bool forward = input.IsKeyDown(rw::input::Key::W);
    const bool backward = input.IsKeyDown(rw::input::Key::S);
    const bool left = input.IsKeyDown(rw::input::Key::A);
    const bool right = input.IsKeyDown(rw::input::Key::D);
    const bool isMoving = forward || backward || left || right;

    m_vitals.Update(deltaSeconds, input.IsKeyDown(rw::input::Key::Shift), isMoving);

    const float speed = m_vitals.IsSprinting() ? sprintSpeed : walkSpeed;
    const float distance = speed * deltaSeconds;
    const rw::math::Vec3 cameraForward = m_camera.Forward();
    const rw::math::Vec3 planarForward = rw::math::Normalize({ cameraForward.x, 0.0F, cameraForward.z });
    const rw::math::Vec3 planarRight = rw::math::Normalize({ m_camera.Right().x, 0.0F, m_camera.Right().z });

    if (forward) {
        m_camera.position += planarForward * distance;
    }
    if (backward) {
        m_camera.position -= planarForward * distance;
    }
    if (right) {
        m_camera.position += planarRight * distance;
    }
    if (left) {
        m_camera.position -= planarRight * distance;
    }
}

const rw::scene::Camera& PlayerController::Camera() const
{
    return m_camera;
}

const PlayerVitals& PlayerController::Vitals() const
{
    return m_vitals;
}

PlayerVitals& PlayerController::Vitals()
{
    return m_vitals;
}

} // namespace rw::game
