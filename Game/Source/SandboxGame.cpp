#include "Game/Source/SandboxGame.h"

#include <algorithm>

namespace rw::game {

SandboxGame::SandboxGame()
{
    rw::scene::SceneObject grid;
    grid.name = "Milestone 1 Debug Grid";
    grid.primitive = rw::scene::DebugPrimitive::Grid;
    grid.color = { 0.25F, 0.28F, 0.32F };
    m_scene.AddObject(grid);

    rw::scene::SceneObject cube;
    cube.name = "Milestone 1 Debug Cube";
    cube.transform.position = { 0.0F, 0.75F, 0.0F };
    cube.transform.scale = { 1.5F, 1.5F, 1.5F };
    cube.color = { 0.25F, 0.72F, 0.95F };
    m_scene.AddObject(cube);
}

void SandboxGame::OnUpdate(float deltaSeconds, const rw::input::InputState& input)
{
    const float moveSpeed = input.IsKeyDown(rw::input::Key::Shift) ? 9.0F : 3.0F;
    const float lookSpeed = 1.8F;

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

    const rw::math::Vec3 forward = m_camera.Forward();
    const rw::math::Vec3 right = m_camera.Right();
    const float distance = moveSpeed * deltaSeconds;

    if (input.IsKeyDown(rw::input::Key::W)) {
        m_camera.position += forward * distance;
    }
    if (input.IsKeyDown(rw::input::Key::S)) {
        m_camera.position += forward * -distance;
    }
    if (input.IsKeyDown(rw::input::Key::D)) {
        m_camera.position += right * distance;
    }
    if (input.IsKeyDown(rw::input::Key::A)) {
        m_camera.position += right * -distance;
    }
}

void SandboxGame::OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window)
{
    renderer.Render(window, m_scene, m_camera);
}

} // namespace rw::game
