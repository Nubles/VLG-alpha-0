#include "Game/Source/SandboxGame.h"

#include "Engine/Core/Logger.h"
#include "Game/Source/AI/EnemyBrain.h"
#include "Game/Source/Biomes/BiomeLayoutBuilder.h"
#include "Game/Source/Crafting/CraftingService.h"
#include "Game/Source/Debug/DebugFormatting.h"
#include "Game/Source/Debug/DebugHelp.h"
#include "Game/Source/Objectives/ShrineObjective.h"
#include "Game/Source/Progression/ProgressionFlag.h"
#include "Game/Source/SaveLoad/SaveGameService.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace rw::game {

SandboxGame::SandboxGame()
    : m_itemDatabase(ItemDatabase::CreateFromFileOrFallback("Game/Data/Items/items.txt"))
    , m_recipeDatabase(RecipeDatabase::CreateFromFileOrFallback("Game/Data/Recipes/recipes.txt"))
    , m_biomeDatabase(BiomeDatabase::CreateStarterBiomes())
    , m_buildableDatabase(BuildableDatabase::CreateFromFileOrFallback("Game/Data/Buildables/buildables.txt"))
    , m_inventory(16)
    , m_hotbar(8)
    , m_realmWisp(EnemyAgent::CreateRealmWisp())
{
    LoadMistwoodHollow();
}

void SandboxGame::OnUpdate(float deltaSeconds, rw::input::InputState& input)
{
    UpdateMouseLook(input);
    m_player.Update(deltaSeconds, input);
    UpdateInteractionTarget(input);
    UpdateDebugItemGrants(input);
    UpdateDebugCrafting(input);
    UpdateBuildPlacement(input);
    UpdateCombatAndEnemies(deltaSeconds, input);
    UpdateSaveLoad(input);
    UpdateDebugHelp(input);
}

void SandboxGame::OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window)
{
    renderer.Render(window, m_scene, m_player.Camera());
    m_hud.Render(renderer, window, BuildHudState());
    renderer.Present(window);
}

std::string SandboxGame::DebugTitle() const
{
    std::ostringstream text;
    text << (m_currentBiome != nullptr ? m_currentBiome->displayName : "No Biome")
         << " | HP " << std::fixed << std::setprecision(0) << m_player.Vitals().Health()
         << " ST " << std::setprecision(0) << m_player.Vitals().Stamina()
         << " | target " << TargetName()
         << " | inv " << InventorySummary()
         << " | enemy " << ToString(m_realmWisp.state)
         << " " << std::setprecision(0) << m_realmWisp.health.Health()
         << " | obj " << ObjectiveStatus();

    text << " | mouse " << (m_playerMouseLookEnabled ? "on" : "off");

    if (m_buildPlacement.BuildModeActive()) {
        text << " | build " << SelectedBuildableName();
    }

    text << " | msg " << m_messages.Summary();

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
            if (m_currentTarget->name == "Realm Fracture") {
                InteractWithRealmFracture();
            } else {
                m_lastInteractionMessage = "Interacted: " + m_currentTarget->name;
                PushDebugMessage(m_currentTarget->debugMessage);
            }
        } else {
            m_lastInteractionMessage = "Interacted: None";
            PushDebugMessage("Interaction attempted with no target.");
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
    if (input.WasKeyPressed(rw::input::Key::F11)) {
        CraftDebugRecipe("realm_anchor");
    }
    if (input.WasKeyPressed(rw::input::Key::F12)) {
        ResetProgressionState();
    }
}

void SandboxGame::UpdateBuildPlacement(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::B)) {
        m_buildPlacement.ToggleBuildMode();
        m_lastBuildMessage = m_buildPlacement.BuildModeActive() ? "Build mode on" : "Build mode off";
        PushDebugMessage(m_lastBuildMessage);
    }

    if (input.WasKeyPressed(rw::input::Key::Backspace)) {
        m_buildPlacement.CancelBuildMode();
        m_lastBuildMessage = "Build mode cancelled";
        PushDebugMessage(m_lastBuildMessage);
    }

    if (input.WasKeyPressed(rw::input::Key::One)) {
        m_buildPlacement.SelectBuildable("camp_marker");
        m_lastBuildMessage = "Selected Camp Marker";
        PushDebugMessage(m_lastBuildMessage);
    }
    if (input.WasKeyPressed(rw::input::Key::Two)) {
        m_buildPlacement.SelectBuildable("workbench_stub");
        m_lastBuildMessage = "Selected Workbench Stub";
        PushDebugMessage(m_lastBuildMessage);
    }
    if (input.WasKeyPressed(rw::input::Key::Three)) {
        m_buildPlacement.SelectBuildable("simple_wall");
        m_lastBuildMessage = "Selected Simple Wall";
        PushDebugMessage(m_lastBuildMessage);
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
            m_buildableDatabase, m_itemDatabase, m_inventory, m_worldState.PlacedBuildables());
        m_lastBuildMessage = result.message;
        PushDebugMessage(m_lastBuildMessage);
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
        PushDebugMessage(m_lastCombatMessage);
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
            PushDebugMessage(m_lastCombatMessage);
        }
    }

    if (!m_player.Vitals().IsAlive()) {
        m_lastCombatMessage = "Player down";
        PushDebugMessage(m_lastCombatMessage);
    }

    SyncRealmWispVisual();
}

void SandboxGame::UpdateSaveLoad(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::O)) {
        const SaveResult result = SaveGameService::SaveToFile(CaptureSaveData());
        m_lastSaveMessage = result.message;
        PushDebugMessage(m_lastSaveMessage);
    }

    if (input.WasKeyPressed(rw::input::Key::P)) {
        const LoadResult result = SaveGameService::LoadFromFile();
        m_lastSaveMessage = result.message;
        if (result.success) {
            ApplySaveData(result.data);
        }
        PushDebugMessage(m_lastSaveMessage);
    }
}

void SandboxGame::UpdateDebugHelp(const rw::input::InputState& input)
{
    if (input.WasKeyPressed(rw::input::Key::H)) {
        PushDebugMessage(DebugHelpText());
    }
}

void SandboxGame::UpdateMouseLook(rw::input::InputState& input)
{
    if (!input.WasKeyPressed(rw::input::Key::M)) {
        input.SetMouseCaptureEnabled(m_playerMouseLookEnabled);
        return;
    }

    m_playerMouseLookEnabled = !m_playerMouseLookEnabled;
    input.SetMouseCaptureEnabled(m_playerMouseLookEnabled);
    m_lastInputMessage = m_playerMouseLookEnabled ? "Mouse look enabled" : "Mouse look disabled";
    PushDebugMessage(m_lastInputMessage);
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

    PushDebugMessage(m_lastGatherMessage);
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
    PushDebugMessage(m_lastInventoryMessage);

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
        m_recipeDatabase, m_itemDatabase, m_progression, m_inventory, recipeId);
    m_lastCraftMessage = result.message;
    PushDebugMessage(m_lastCraftMessage);

    for (int index = 0; index < static_cast<int>(m_inventory.Slots().size())
         && index < static_cast<int>(m_hotbar.Entries().size()); ++index) {
        if (!m_inventory.Slots()[static_cast<size_t>(index)].IsEmpty()) {
            m_hotbar.AssignSlot(index, index);
        }
    }
}

void SandboxGame::InteractWithRealmFracture()
{
    ObjectiveResult result = ShrineObjective::TryCompleteMistwoodFracture(
        m_inventory, m_progression, m_objectiveState);
    m_lastInteractionMessage = "Interacted: Realm Fracture";
    m_lastProgressionMessage = result.message;

    for (Interactable& interactable : m_interactables) {
        if (interactable.name == "Realm Fracture"
            && m_progression.HasFlag(kMistwoodFractureStabilized)) {
            interactable.debugMessage = "The fracture steadies into a calm, silver-blue shimmer.";
        }
    }

    UpdateRealmFractureVisual();
    PushDebugMessage(m_lastProgressionMessage);
}

void SandboxGame::ResetProgressionState()
{
    m_progression.Clear();
    m_objectiveState.Reset();
    m_lastProgressionMessage = "Progression reset";
    for (Interactable& interactable : m_interactables) {
        if (interactable.name == "Realm Fracture") {
            interactable.debugMessage = "The fracture shimmers, but it is not stable enough to enter.";
        }
    }
    UpdateRealmFractureVisual();
    PushDebugMessage(m_lastProgressionMessage);
}

void SandboxGame::UpdateRealmFractureVisual()
{
    for (rw::scene::SceneObject& object : m_scene.MutableObjects()) {
        if (object.name != "Realm Fracture") {
            continue;
        }

        if (m_progression.HasFlag(kMistwoodFractureStabilized)) {
            object.color = { 0.65F, 0.82F, 0.95F };
            object.transform.scale = { 1.1F, 2.5F, 0.45F };
        } else {
            object.color = { 0.35F, 0.45F, 0.95F };
            object.transform.scale = { 1.0F, 2.8F, 0.35F };
        }
        return;
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
    PlacedBuildable registered = placed;
    if (registered.instanceId.empty()) {
        registered.instanceId = m_worldState.AllocatePlacedBuildableId();
    }
    m_worldState.RegisterPlacedBuildable(registered);

    rw::scene::SceneObject object;
    object.name = registered.instanceId;
    object.transform = registered.transform;
    object.primitive = registered.primitive;
    object.color = { 0.82F, 0.70F, 0.40F };
    m_scene.AddObject(object);
}

std::string SandboxGame::SelectedBuildableName() const
{
    const BuildableDefinition* selected = m_buildableDatabase.FindById(
        m_buildPlacement.SelectedBuildableId());
    return selected != nullptr ? selected->displayName : "None";
}

std::string SandboxGame::TargetName() const
{
    if (m_currentGatherableIndex >= 0) {
        return m_gatherableNodes[static_cast<size_t>(m_currentGatherableIndex)].name;
    }
    return m_currentTarget != nullptr ? m_currentTarget->name : "None";
}

std::string SandboxGame::ObjectiveStatus() const
{
    if (m_progression.HasFlag(kMistwoodFractureStabilized)) {
        return "fracture stable";
    }
    if (m_progression.HasFlag(kMistwoodFractureDiscovered)) {
        return "fracture found";
    }
    return "fracture unknown";
}

HudState SandboxGame::BuildHudState() const
{
    HudState state;
    state.health = m_player.Vitals().Health();
    state.maxHealth = 100.0F;
    state.stamina = m_player.Vitals().Stamina();
    state.maxStamina = 100.0F;
    state.currentTargetName = TargetName();
    state.inventorySummary = InventorySummary();
    state.enemyName = m_realmWisp.name;
    state.enemyHealth = m_realmWisp.health.Health();
    state.enemyMaxHealth = m_realmWisp.health.MaxHealth();
    state.enemyState = ToString(m_realmWisp.state);
    state.objectiveStatus = ObjectiveStatus();
    state.buildModeActive = m_buildPlacement.BuildModeActive();
    state.selectedBuildableName = SelectedBuildableName();
    state.messages = m_messages.Messages();
    state.saveLoadMessage = m_lastSaveMessage;
    state.mouseLookEnabled = m_playerMouseLookEnabled;
    return state;
}

void SandboxGame::PushDebugMessage(const std::string& message)
{
    m_messages.Push(message);
    rw::core::Logger::Info(message);
}

void SandboxGame::ResetRealmWisp()
{
    m_realmWisp = EnemyAgent::CreateRealmWisp();
    if (m_currentBiome != nullptr && m_currentBiome->id == "mistwood_hollow") {
        m_realmWisp.transform.position = { 1.2F, 0.7F, -5.0F };
    }
    m_lastCombatMessage = "Realm Wisp reset";
    PushDebugMessage(m_lastCombatMessage);
    SyncRealmWispVisual();
}

void SandboxGame::LoadMistwoodHollow()
{
    m_currentBiome = m_biomeDatabase.FindById("mistwood_hollow");
    const BiomeLayout layout = BiomeLayoutBuilder::BuildMistwoodHollow();

    m_player.SetPosition(layout.playerSpawn);
    for (const rw::scene::SceneObject& object : layout.sceneObjects) {
        m_scene.AddObject(object);
    }

    m_interactables = layout.interactables;
    m_gatherableNodes = layout.gatherableNodes;
    m_realmWisp = layout.realmWisp;

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
}

SaveData SandboxGame::CaptureSaveData() const
{
    SaveData data;
    data.biomeId = m_currentBiome != nullptr ? m_currentBiome->id : "mistwood_hollow";
    data.playerPosition = m_player.Camera().position;
    data.playerYawRadians = m_player.Camera().yawRadians;
    data.playerPitchRadians = m_player.Camera().pitchRadians;
    data.playerHealth = m_player.Vitals().Health();
    data.playerStamina = m_player.Vitals().Stamina();
    data.inventorySlots = m_inventory.Slots();
    data.progressionFlags = m_progression.Flags();
    data.mistwoodFractureStabilized = m_progression.HasFlag(kMistwoodFractureStabilized);
    data.mistwoodObjectiveCompleted = m_objectiveState.IsCompleted("mistwood_fracture_shrine");
    data.realmWispAlive = m_realmWisp.health.IsAlive();
    data.realmWispPosition = m_realmWisp.transform.position;
    data.depletedGatherableIds = m_worldState.DepletedGatherableIds(m_gatherableNodes);

    for (const PlacedBuildable& placed : m_worldState.PlacedBuildables()) {
        data.placedBuildables.push_back({
            placed.instanceId,
            placed.buildableId,
            placed.transform.position,
            placed.transform.rotationEuler.y,
        });
    }

    return data;
}

void SandboxGame::ApplySaveData(const SaveData& data)
{
    m_player.SetPosition(data.playerPosition);
    m_player.SetLook(data.playerYawRadians, data.playerPitchRadians);
    m_player.Vitals().SetHealth(data.playerHealth);
    m_player.Vitals().SetStamina(data.playerStamina);

    std::vector<ItemStack> slots = data.inventorySlots;
    slots.resize(static_cast<size_t>(m_inventory.SlotCount()));
    m_inventory.ReplaceSlots(slots);

    m_progression.Clear();
    for (const std::string& flag : data.progressionFlags) {
        m_progression.SetFlag(flag);
    }
    if (data.mistwoodFractureStabilized) {
        m_progression.SetFlag(kMistwoodFractureStabilized);
    }

    m_objectiveState.Reset();
    if (data.mistwoodObjectiveCompleted) {
        m_objectiveState.Complete("mistwood_fracture_shrine");
        m_progression.SetFlag(kMistwoodShrineCompleted);
    }
    if (data.mistwoodFractureStabilized) {
        m_progression.SetFlag(kRecipeRealmAnchorUnlocked);
    }

    auto& objects = m_scene.MutableObjects();
    for (const PlacedBuildable& placed : m_worldState.PlacedBuildables()) {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                          [&placed](const rw::scene::SceneObject& object) {
                              return object.name == placed.instanceId;
                          }),
            objects.end());
    }
    std::vector<PlacedBuildable> loadedBuildables;

    for (const SavedBuildable& saved : data.placedBuildables) {
        const BuildableDefinition* definition = m_buildableDatabase.FindById(saved.buildableId);
        if (definition == nullptr) {
            continue;
        }

        PlacedBuildable placed;
        placed.instanceId = saved.instanceId;
        placed.buildableId = definition->id;
        placed.displayName = definition->displayName;
        placed.transform.position = saved.position;
        placed.transform.rotationEuler = { 0.0F, saved.yawRadians, 0.0F };
        placed.transform.scale = definition->placedScale;
        placed.primitive = definition->primitive;
        placed.placementRadius = definition->placementRadius;
        loadedBuildables.push_back(placed);
    }
    m_worldState.ReplacePlacedBuildables(loadedBuildables);

    for (const PlacedBuildable& placed : m_worldState.PlacedBuildables()) {
        rw::scene::SceneObject object;
        object.name = placed.instanceId;
        object.transform = placed.transform;
        object.primitive = placed.primitive;
        object.color = { 0.82F, 0.70F, 0.40F };
        m_scene.AddObject(object);
    }

    m_worldState.ApplyDepletedGatherableIds(m_gatherableNodes, data.depletedGatherableIds);

    m_realmWisp = EnemyAgent::CreateRealmWisp();
    m_realmWisp.transform.position = data.realmWispPosition;
    if (!data.realmWispAlive) {
        m_realmWisp.health.ApplyDamage({ 9999.0F, "Load" });
        m_realmWisp.lootDropped = true;
    }

    UpdateRealmFractureVisual();
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
    return CompactInventorySummary(m_itemDatabase, m_inventory);
}

} // namespace rw::game
