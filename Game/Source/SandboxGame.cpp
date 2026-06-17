#include "Game/Source/SandboxGame.h"

#include "Engine/Core/Logger.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

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
    cube.transform.position = { -2.5F, 0.75F, -0.5F };
    cube.transform.scale = { 1.5F, 1.5F, 1.5F };
    cube.color = { 0.25F, 0.72F, 0.95F };
    m_scene.AddObject(cube);

    Interactable ancientStone;
    ancientStone.name = "Ancient Stone";
    ancientStone.transform.position = { 0.0F, 0.75F, 0.0F };
    ancientStone.transform.scale = { 1.2F, 1.2F, 1.2F };
    ancientStone.interactionRadius = 1.1F;
    ancientStone.debugMessage = "Ancient Stone hums with dormant realm energy.";
    m_interactables.push_back(ancientStone);

    rw::scene::SceneObject stoneVisual;
    stoneVisual.name = ancientStone.name;
    stoneVisual.transform = ancientStone.transform;
    stoneVisual.color = { 0.72F, 0.66F, 0.48F };
    m_scene.AddObject(stoneVisual);
}

void SandboxGame::OnUpdate(float deltaSeconds, const rw::input::InputState& input)
{
    m_player.Update(deltaSeconds, input);
    UpdateInteractionTarget(input);
}

void SandboxGame::OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window)
{
    renderer.Render(window, m_scene, m_player.Camera());
}

std::string SandboxGame::DebugTitle() const
{
    std::ostringstream text;
    text << "health " << std::fixed << std::setprecision(0) << m_player.Vitals().Health()
         << " | stamina " << std::setprecision(0) << m_player.Vitals().Stamina()
         << " | target " << (m_currentTarget != nullptr ? m_currentTarget->name : "None");

    if (!m_lastInteractionMessage.empty()) {
        text << " | " << m_lastInteractionMessage;
    }

    return text.str();
}

void SandboxGame::UpdateInteractionTarget(const rw::input::InputState& input)
{
    const rw::scene::Camera& camera = m_player.Camera();
    const InteractionRay ray {
        camera.position,
        camera.Forward(),
        6.0F,
    };

    m_currentTarget = nullptr;
    float nearestDistance = ray.maxDistance;

    for (const Interactable& interactable : m_interactables) {
        if (!interactable.HitBy(ray)) {
            continue;
        }

        const float distance = rw::math::Length(interactable.transform.position - camera.position);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            m_currentTarget = &interactable;
        }
    }

    if (input.WasKeyPressed(rw::input::Key::E)) {
        if (m_currentTarget != nullptr) {
            m_lastInteractionMessage = "Interacted: " + m_currentTarget->name;
            rw::core::Logger::Info(m_currentTarget->debugMessage);
        } else {
            m_lastInteractionMessage = "Interacted: None";
            rw::core::Logger::Info("Interaction attempted with no target.");
        }
    }
}

} // namespace rw::game
