#include "Game/Source/SandboxGame.h"

#include "Engine/Core/Logger.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace rw::game {

SandboxGame::SandboxGame()
    : m_itemDatabase(ItemDatabase::CreateStarterDatabase())
    , m_inventory(16)
    , m_hotbar(8)
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

    GatherableNode branchPile;
    branchPile.name = "Branch Pile";
    branchPile.transform.position = { 2.5F, 0.35F, -1.0F };
    branchPile.transform.scale = { 0.8F, 0.5F, 0.8F };
    branchPile.interactionRadius = 1.0F;
    branchPile.remainingGatherCount = 2;
    branchPile.dropTable.AddDrop("wood", 4, 4);
    branchPile.debugMessage = "Gathered dry branches.";
    AddGatherableNode(branchPile, { 0.45F, 0.28F, 0.13F });

    GatherableNode looseStone;
    looseStone.name = "Loose Stone";
    looseStone.transform.position = { 4.0F, 0.35F, 0.5F };
    looseStone.transform.scale = { 0.7F, 0.5F, 0.7F };
    looseStone.interactionRadius = 0.9F;
    looseStone.remainingGatherCount = 1;
    looseStone.dropTable.AddDrop("stone", 3, 3);
    looseStone.debugMessage = "Gathered loose stone.";
    AddGatherableNode(looseStone, { 0.42F, 0.42F, 0.45F });

    GatherableNode fiberPlant;
    fiberPlant.name = "Fiber Plant";
    fiberPlant.transform.position = { 1.5F, 0.45F, 2.0F };
    fiberPlant.transform.scale = { 0.5F, 0.9F, 0.5F };
    fiberPlant.interactionRadius = 0.9F;
    fiberPlant.remainingGatherCount = 2;
    fiberPlant.dropTable.AddDrop("fiber", 5, 5);
    fiberPlant.debugMessage = "Gathered tough plant fiber.";
    AddGatherableNode(fiberPlant, { 0.28F, 0.65F, 0.32F });

    GatherableNode crackedRock;
    crackedRock.name = "Cracked Rock";
    crackedRock.transform.position = { -1.0F, 0.55F, 2.5F };
    crackedRock.transform.scale = { 1.0F, 1.0F, 1.0F };
    crackedRock.interactionRadius = 1.1F;
    crackedRock.remainingGatherCount = 1;
    crackedRock.dropTable.AddDrop("stone", 8, 8);
    crackedRock.requiredToolId = "primitive_tool";
    crackedRock.debugMessage = "Chipped stone from the cracked rock.";
    AddGatherableNode(crackedRock, { 0.30F, 0.32F, 0.36F });
}

void SandboxGame::OnUpdate(float deltaSeconds, const rw::input::InputState& input)
{
    m_player.Update(deltaSeconds, input);
    UpdateInteractionTarget(input);
    UpdateDebugItemGrants(input);
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
         << " | target ";

    if (m_currentGatherableIndex >= 0) {
        text << m_gatherableNodes[static_cast<size_t>(m_currentGatherableIndex)].name;
    } else {
        text << (m_currentTarget != nullptr ? m_currentTarget->name : "None");
    }

    if (!m_lastInteractionMessage.empty()) {
        text << " | " << m_lastInteractionMessage;
    }

    text << " | inv " << InventorySummary();

    if (!m_lastInventoryMessage.empty()) {
        text << " | " << m_lastInventoryMessage;
    }

    if (!m_lastGatherMessage.empty()) {
        text << " | " << m_lastGatherMessage;
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
    m_currentGatherableIndex = -1;
    float nearestDistance = ray.maxDistance;

    for (int index = 0; index < static_cast<int>(m_gatherableNodes.size()); ++index) {
        const GatherableNode& node = m_gatherableNodes[static_cast<size_t>(index)];
        if (!node.HitBy(ray)) {
            continue;
        }

        const float distance = rw::math::Length(node.transform.position - camera.position);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            m_currentTarget = nullptr;
            m_currentGatherableIndex = index;
        }
    }

    for (const Interactable& interactable : m_interactables) {
        if (!interactable.HitBy(ray)) {
            continue;
        }

        const float distance = rw::math::Length(interactable.transform.position - camera.position);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            m_currentTarget = &interactable;
            m_currentGatherableIndex = -1;
        }
    }

    if (input.WasKeyPressed(rw::input::Key::E)) {
        if (m_currentGatherableIndex >= 0) {
            GatherTargetNode(m_gatherableNodes[static_cast<size_t>(m_currentGatherableIndex)]);
        } else if (m_currentTarget != nullptr) {
            m_lastInteractionMessage = "Interacted: " + m_currentTarget->name;
            rw::core::Logger::Info(m_currentTarget->debugMessage);
        } else {
            m_lastInteractionMessage = "Interacted: None";
            rw::core::Logger::Info("Interaction attempted with no target.");
        }
    }
}

void SandboxGame::UpdateDebugItemGrants(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::F1)) {
        GrantDebugItem("wood", 10);
    }
    if (input.WasKeyPressed(rw::input::Key::F2)) {
        GrantDebugItem("stone", 10);
    }
    if (input.WasKeyPressed(rw::input::Key::F3)) {
        GrantDebugItem("fiber", 10);
    }
    if (input.WasKeyPressed(rw::input::Key::F4)) {
        GrantDebugItem("raw_food", 5);
    }
    if (input.WasKeyPressed(rw::input::Key::F5)) {
        GrantDebugItem("primitive_tool", 1);
    }
}

void SandboxGame::GatherTargetNode(GatherableNode& node)
{
    const GatheringResult result = node.Gather(m_itemDatabase, m_inventory);
    m_lastGatherMessage = result.message;

    if (result.success && !result.drops.empty()) {
        std::ostringstream drops;
        drops << result.message << " Drops:";
        for (const ItemStack& drop : result.drops) {
            drops << " " << drop.itemId << "x" << drop.quantity;
        }
        if (result.leftoverQuantity > 0) {
            drops << " leftover:" << result.leftoverQuantity;
        }
        m_lastGatherMessage = drops.str();
    }

    rw::core::Logger::Info(m_lastGatherMessage);
}

void SandboxGame::AddGatherableNode(const GatherableNode& node, const rw::math::Vec3& color)
{
    m_gatherableNodes.push_back(node);

    rw::scene::SceneObject visual;
    visual.name = node.name;
    visual.transform = node.transform;
    visual.color = color;
    m_scene.AddObject(visual);
}

void SandboxGame::GrantDebugItem(const std::string& itemId, int quantity)
{
    const ItemDefinition* definition = m_itemDatabase.FindById(itemId);
    const int leftover = m_inventory.AddItem(m_itemDatabase, itemId, quantity);
    const int added = quantity - leftover;

    std::ostringstream message;
    message << "Granted " << added << " "
            << (definition != nullptr ? definition->displayName : itemId);
    if (leftover > 0) {
        message << " (" << leftover << " left over)";
    }

    m_lastInventoryMessage = message.str();
    rw::core::Logger::Info(m_lastInventoryMessage);

    for (int index = 0; index < static_cast<int>(m_inventory.Slots().size())
         && index < static_cast<int>(m_hotbar.Entries().size()); ++index) {
        if (!m_inventory.Slots()[static_cast<size_t>(index)].IsEmpty()) {
            m_hotbar.AssignSlot(index, index);
        }
    }
}

std::string SandboxGame::InventorySummary() const
{
    std::ostringstream summary;
    bool wroteAny = false;

    for (const ItemDefinition& definition : m_itemDatabase.Definitions()) {
        const int quantity = m_inventory.TotalQuantity(definition.id);
        if (quantity <= 0) {
            continue;
        }

        if (wroteAny) {
            summary << ", ";
        }

        summary << definition.id << ":" << quantity;
        wroteAny = true;
    }

    if (!wroteAny) {
        return "empty";
    }

    return summary.str();
}

} // namespace rw::game
