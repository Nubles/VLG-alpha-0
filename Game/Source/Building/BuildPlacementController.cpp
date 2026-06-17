#include "Game/Source/Building/BuildPlacementController.h"

#include "Engine/Math/Vec3.h"
#include "Game/Source/Building/BuildableDatabase.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"

namespace rw::game {

namespace {

constexpr float kMinimumPlacementDistance = 1.0F;
constexpr float kMaximumPlacementDistance = 8.0F;

} // namespace

void BuildPlacementController::ToggleBuildMode()
{
    m_buildModeActive = !m_buildModeActive;
}

void BuildPlacementController::CancelBuildMode()
{
    m_buildModeActive = false;
}

void BuildPlacementController::SelectBuildable(const std::string& buildableId)
{
    m_selectedBuildableId = buildableId;
    m_buildModeActive = true;
}

void BuildPlacementController::Rotate(float yawDeltaRadians)
{
    m_yawRadians += yawDeltaRadians;
}

void BuildPlacementController::UpdatePreview(
    const BuildableDatabase& buildables,
    const rw::scene::Camera& camera)
{
    m_lastCameraPosition = camera.position;

    const BuildableDefinition* buildable = buildables.FindById(m_selectedBuildableId);
    if (buildable == nullptr) {
        return;
    }

    const rw::math::Vec3 forward = camera.Forward();
    m_previewTransform.position = camera.position + forward * buildable->placementDistance;
    m_previewTransform.scale = buildable->previewScale;
    m_previewTransform.rotationEuler = { 0.0F, m_yawRadians, 0.0F };
}

BuildPlacementResult BuildPlacementController::TryPlace(
    const BuildableDatabase& buildables,
    const ItemDatabase& items,
    Inventory& inventory,
    const std::vector<PlacedBuildable>& placedBuildables)
{
    (void)items;

    if (!m_buildModeActive) {
        return { false, BuildPlacementFailureReason::BuildModeInactive, "Build failed: mode inactive", {} };
    }

    const BuildableDefinition* buildable = buildables.FindById(m_selectedBuildableId);
    if (m_selectedBuildableId.empty()) {
        return { false, BuildPlacementFailureReason::NoBuildableSelected, "Build failed: no buildable selected", {} };
    }

    if (buildable == nullptr) {
        return { false, BuildPlacementFailureReason::UnknownBuildable, "Build failed: unknown buildable", {} };
    }

    if (!HasCost(buildables, inventory)) {
        return { false, BuildPlacementFailureReason::MissingCost, "Build failed: missing cost for " + buildable->displayName, {} };
    }

    if (!IsPreviewPlacementValid(m_lastCameraPosition)) {
        return { false, BuildPlacementFailureReason::InvalidPlacement, "Build failed: invalid placement", {} };
    }

    if (OverlapsExisting(buildables, placedBuildables)) {
        return { false, BuildPlacementFailureReason::Overlap, "Build failed: placement overlaps", {} };
    }

    for (const BuildCost& cost : buildable->costs) {
        inventory.RemoveItem(cost.itemId, cost.quantity);
    }

    PlacedBuildable placed;
    placed.buildableId = buildable->id;
    placed.displayName = buildable->displayName;
    placed.transform = m_previewTransform;
    placed.transform.scale = buildable->placedScale;
    placed.primitive = buildable->primitive;
    placed.placementRadius = buildable->placementRadius;

    return {
        true,
        BuildPlacementFailureReason::None,
        "Placed " + buildable->displayName,
        placed,
    };
}

bool BuildPlacementController::BuildModeActive() const
{
    return m_buildModeActive;
}

const std::string& BuildPlacementController::SelectedBuildableId() const
{
    return m_selectedBuildableId;
}

float BuildPlacementController::YawRadians() const
{
    return m_yawRadians;
}

const rw::math::Transform& BuildPlacementController::PreviewTransform() const
{
    return m_previewTransform;
}

bool BuildPlacementController::HasCost(
    const BuildableDatabase& buildables,
    const Inventory& inventory) const
{
    const BuildableDefinition* buildable = buildables.FindById(m_selectedBuildableId);
    if (buildable == nullptr) {
        return false;
    }

    for (const BuildCost& cost : buildable->costs) {
        if (!inventory.Contains(cost.itemId, cost.quantity)) {
            return false;
        }
    }

    return true;
}

bool BuildPlacementController::IsPreviewPlacementValid(const rw::math::Vec3& cameraPosition) const
{
    const float distance = rw::math::Length(m_previewTransform.position - cameraPosition);
    return distance >= kMinimumPlacementDistance && distance <= kMaximumPlacementDistance;
}

bool BuildPlacementController::OverlapsExisting(
    const BuildableDatabase& buildables,
    const std::vector<PlacedBuildable>& placedBuildables) const
{
    const BuildableDefinition* buildable = buildables.FindById(m_selectedBuildableId);
    if (buildable == nullptr) {
        return false;
    }

    for (const PlacedBuildable& placed : placedBuildables) {
        const float distance = rw::math::Length(m_previewTransform.position - placed.transform.position);
        if (distance < buildable->placementRadius + placed.placementRadius) {
            return true;
        }
    }

    return false;
}

} // namespace rw::game
