#include "Game/Source/SandboxGame.h"

#include "Engine/Core/Logger.h"
#include "Game/Source/AI/EnemyBrain.h"
#include "Game/Source/Crafting/CraftingService.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace rw::game {

SandboxGame::SandboxGame()
    : m_itemDatabase(ItemDatabase::CreateStarterDatabase())
    , m_recipeDatabase(RecipeDatabase::CreateStarterRecipes())
    , m_buildableDatabase(BuildableDatabase::CreateStarterBuildables())
    , m_inventory(16)
    , m_hotbar(8)
    , m_realmWisp(EnemyAgent::CreateRealmWisp())
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

    rw::scene::SceneObject buildPreview;
    buildPreview.name = "Build Preview";
    buildPreview.transform.scale = { 0.0F, 0.0F, 0.0F };
    buildPreview.color = { 0.30F, 0.85F, 0.65F };
    m_scene.AddObject(buildPreview);

    rw::scene::SceneObject wispVisual;
    wispVisual.name = m_realmWisp.name;
    wispVisual.transform = m_realmWisp.transform;
    wispVisual.color = { 0.75F, 0.30F, 0.95F };
    m_scene.AddObject(wispVisual);

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
    UpdateDebugCrafting(input);
    UpdateBuildPlacement(input);
    UpdateCombatAndEnemies(deltaSeconds, input);
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
         << " | build " << (m_buildPlacement.BuildModeActive() ? "On" : "Off")
         << " " << SelectedBuildableName()
         << " | enemy " << ToString(m_realmWisp.state)
         << " " << std::setprecision(0) << m_realmWisp.health.Health()
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

    if (!m_lastCraftMessage.empty()) {
        text << " | " << m_lastCraftMessage;
    }

    if (!m_lastBuildMessage.empty()) {
        text << " | " << m_lastBuildMessage;
    }

    if (!m_lastCombatMessage.empty()) {
        text << " | " << m_lastCombatMessage;
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

void SandboxGame::UpdateDebugCrafting(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::F6)) {
        CraftDebugRecipe("primitive_tool");
    }
    if (input.WasKeyPressed(rw::input::Key::F7)) {
        CraftDebugRecipe("camp_bundle");
    }
    if (input.WasKeyPressed(rw::input::Key::F8)) {
        CraftDebugRecipe("simple_meal");
    }
    if (input.WasKeyPressed(rw::input::Key::F9)) {
        CraftDebugRecipe("workbench_kit");
    }
}

void SandboxGame::UpdateBuildPlacement(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::B)) {
        m_buildPlacement.ToggleBuildMode();
        m_lastBuildMessage = m_buildPlacement.BuildModeActive() ? "Build mode on" : "Build mode off";
        rw::core::Logger::Info(m_lastBuildMessage);
    }

    if (input.WasKeyPressed(rw::input::Key::Backspace)) {
        m_buildPlacement.CancelBuildMode();
        m_lastBuildMessage = "Build mode cancelled";
        rw::core::Logger::Info(m_lastBuildMessage);
    }

    if (input.WasKeyPressed(rw::input::Key::One)) {
        m_buildPlacement.SelectBuildable("camp_marker");
        m_lastBuildMessage = "Selected Camp Marker";
    }
    if (input.WasKeyPressed(rw::input::Key::Two)) {
        m_buildPlacement.SelectBuildable("workbench_stub");
        m_lastBuildMessage = "Selected Workbench Stub";
    }
    if (input.WasKeyPressed(rw::input::Key::Three)) {
        m_buildPlacement.SelectBuildable("simple_wall");
        m_lastBuildMessage = "Selected Simple Wall";
    }

    if (input.WasKeyPressed(rw::input::Key::Q)) {
        m_buildPlacement.Rotate(-0.2617994F);
    }
    if (input.WasKeyPressed(rw::input::Key::R)) {
        m_buildPlacement.Rotate(0.2617994F);
    }

    m_buildPlacement.UpdatePreview(m_buildableDatabase, m_player.Camera());
    SyncBuildPreview();

    if (input.WasKeyPressed(rw::input::Key::Enter)) {
        const BuildPlacementResult result = m_buildPlacement.TryPlace(
            m_buildableDatabase, m_itemDatabase, m_inventory, m_placedBuildables);
        m_lastBuildMessage = result.message;
        rw::core::Logger::Info(m_lastBuildMessage);
        if (result.success) {
            AddPlacedBuildable(result.placed);
        }
    }
}

void SandboxGame::UpdateCombatAndEnemies(float deltaSeconds, const rw::input::InputState& input)
{
    m_playerCombat.Update(deltaSeconds);

    if (input.WasKeyPressed(rw::input::Key::F10)) {
        ResetRealmWisp();
    }

    if (input.WasKeyPressed(rw::input::Key::C)) {
        const PlayerAttackResult attack = m_playerCombat.TryMeleeAttack(
            m_player.Camera(), m_realmWisp.transform.position, m_realmWisp.health);
        if (attack.onCooldown) {
            m_lastCombatMessage = "Attack cooling down";
        } else if (attack.hit) {
            m_lastCombatMessage = "Hit " + m_realmWisp.name;
            if (!m_realmWisp.health.IsAlive()) {
                EnemyLootResult loot = m_realmWisp.DropLoot(m_itemDatabase, m_inventory);
                if (!loot.message.empty()) {
                    m_lastCombatMessage = loot.message;
                }
            }
        } else {
            m_lastCombatMessage = "Attack missed";
        }
        rw::core::Logger::Info(m_lastCombatMessage);
    }

    const bool wasAlive = m_realmWisp.health.IsAlive();
    EnemyUpdateResult enemyUpdate = m_realmWisp.Update(
        deltaSeconds, m_player.Camera().position, m_player.Vitals());
    if (!enemyUpdate.message.empty()) {
        m_lastCombatMessage = enemyUpdate.message;
    }

    if (wasAlive && !m_realmWisp.health.IsAlive()) {
        EnemyLootResult loot = m_realmWisp.DropLoot(m_itemDatabase, m_inventory);
        if (!loot.message.empty()) {
            m_lastCombatMessage = loot.message;
            rw::core::Logger::Info(m_lastCombatMessage);
        }
    }

    if (!m_player.Vitals().IsAlive()) {
        m_lastCombatMessage = "Player down";
    }

    SyncRealmWispVisual();
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

void SandboxGame::CraftDebugRecipe(const std::string& recipeId)
{
    const CraftingResult result = CraftingService::Craft(
        m_recipeDatabase, m_itemDatabase, m_inventory, recipeId);
    m_lastCraftMessage = result.message;
    rw::core::Logger::Info(m_lastCraftMessage);

    for (int index = 0; index < static_cast<int>(m_inventory.Slots().size())
         && index < static_cast<int>(m_hotbar.Entries().size()); ++index) {
        if (!m_inventory.Slots()[static_cast<size_t>(index)].IsEmpty()) {
            m_hotbar.AssignSlot(index, index);
        }
    }
}

void SandboxGame::SyncBuildPreview()
{
    const BuildableDefinition* selected = m_buildableDatabase.FindById(
        m_buildPlacement.SelectedBuildableId());

    for (rw::scene::SceneObject& object : m_scene.MutableObjects()) {
        if (object.name != "Build Preview") {
            continue;
        }

        if (!m_buildPlacement.BuildModeActive() || selected == nullptr) {
            object.transform.scale = { 0.0F, 0.0F, 0.0F };
            return;
        }

        object.primitive = selected->primitive;
        object.transform = m_buildPlacement.PreviewTransform();
        object.color = { 0.30F, 0.85F, 0.65F };
        return;
    }
}

void SandboxGame::AddPlacedBuildable(const PlacedBuildable& placed)
{
    m_placedBuildables.push_back(placed);

    rw::scene::SceneObject object;
    object.name = placed.displayName;
    object.transform = placed.transform;
    object.primitive = placed.primitive;
    object.color = { 0.82F, 0.70F, 0.40F };
    m_scene.AddObject(object);
}

std::string SandboxGame::SelectedBuildableName() const
{
    const BuildableDefinition* selected = m_buildableDatabase.FindById(
        m_buildPlacement.SelectedBuildableId());
    return selected != nullptr ? selected->displayName : "None";
}

void SandboxGame::ResetRealmWisp()
{
    m_realmWisp = EnemyAgent::CreateRealmWisp();
    m_lastCombatMessage = "Realm Wisp reset";
    rw::core::Logger::Info(m_lastCombatMessage);
    SyncRealmWispVisual();
}

void SandboxGame::SyncRealmWispVisual()
{
    for (rw::scene::SceneObject& object : m_scene.MutableObjects()) {
        if (object.name != m_realmWisp.name) {
            continue;
        }

        object.transform = m_realmWisp.transform;
        if (m_realmWisp.health.IsAlive()) {
            object.color = { 0.75F, 0.30F, 0.95F };
        } else {
            object.transform.scale = { 0.25F, 0.12F, 0.25F };
            object.color = { 0.18F, 0.10F, 0.22F };
        }
        return;
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
