#include "Game/Source/Biomes/BiomeLayoutBuilder.h"

namespace rw::game {

namespace {

void AddSceneObject(
    BiomeLayout& layout,
    const char* name,
    const rw::math::Vec3& position,
    const rw::math::Vec3& scale,
    const rw::math::Vec3& color)
{
    rw::scene::SceneObject object;
    object.name = name;
    object.transform.position = position;
    object.transform.scale = scale;
    object.color = color;
    layout.sceneObjects.push_back(object);
}

void AddGatherable(
    BiomeLayout& layout,
    GatherableNode node,
    const rw::math::Vec3& color)
{
    layout.gatherableNodes.push_back(node);
    AddSceneObject(layout, node.name.c_str(), node.transform.position, node.transform.scale, color);
}

} // namespace

BiomeLayout BiomeLayoutBuilder::BuildMistwoodHollow()
{
    BiomeLayout layout;
    layout.playerSpawn = { 0.0F, 1.8F, 7.0F };

    rw::scene::SceneObject grid;
    grid.name = "Mistwood Hollow Debug Grid";
    grid.primitive = rw::scene::DebugPrimitive::Grid;
    grid.color = { 0.20F, 0.27F, 0.24F };
    layout.sceneObjects.push_back(grid);

    AddSceneObject(layout, "Old Root Circle", { -2.5F, 0.45F, 1.5F }, { 2.8F, 0.9F, 2.8F }, { 0.18F, 0.34F, 0.20F });
    AddSceneObject(layout, "Broken Waystone", { 2.0F, 1.0F, -1.5F }, { 0.7F, 2.0F, 0.6F }, { 0.52F, 0.55F, 0.50F });

    Interactable ancientStone;
    ancientStone.name = "Ancient Stone";
    ancientStone.transform.position = { 1.2F, 0.75F, 0.3F };
    ancientStone.transform.scale = { 1.2F, 1.2F, 1.2F };
    ancientStone.interactionRadius = 1.1F;
    ancientStone.debugMessage = "Ancient Stone hums with dormant realm energy.";
    layout.interactables.push_back(ancientStone);
    AddSceneObject(layout, ancientStone.name.c_str(), ancientStone.transform.position, ancientStone.transform.scale, { 0.72F, 0.66F, 0.48F });

    Interactable realmFracture;
    realmFracture.name = "Realm Fracture";
    realmFracture.transform.position = { 0.0F, 1.4F, -6.5F };
    realmFracture.transform.scale = { 1.0F, 2.8F, 0.35F };
    realmFracture.interactionRadius = 1.4F;
    realmFracture.debugMessage = "The fracture shimmers, but it is not stable enough to enter.";
    layout.interactables.push_back(realmFracture);
    AddSceneObject(layout, realmFracture.name.c_str(), realmFracture.transform.position, realmFracture.transform.scale, { 0.35F, 0.45F, 0.95F });

    GatherableNode branchPileA;
    branchPileA.name = "Branch Pile";
    branchPileA.transform.position = { -1.5F, 0.35F, 4.0F };
    branchPileA.transform.scale = { 0.8F, 0.5F, 0.8F };
    branchPileA.interactionRadius = 1.0F;
    branchPileA.remainingGatherCount = 2;
    branchPileA.dropTable.AddDrop("wood", 4, 4);
    branchPileA.debugMessage = "Gathered dry branches.";
    AddGatherable(layout, branchPileA, { 0.45F, 0.28F, 0.13F });

    GatherableNode branchPileB = branchPileA;
    branchPileB.transform.position = { 0.4F, 0.35F, 4.4F };
    AddGatherable(layout, branchPileB, { 0.42F, 0.26F, 0.12F });

    GatherableNode looseStone;
    looseStone.name = "Loose Stone";
    looseStone.transform.position = { 3.5F, 0.35F, 1.0F };
    looseStone.transform.scale = { 0.7F, 0.5F, 0.7F };
    looseStone.interactionRadius = 0.9F;
    looseStone.remainingGatherCount = 1;
    looseStone.dropTable.AddDrop("stone", 3, 3);
    looseStone.debugMessage = "Gathered loose stone.";
    AddGatherable(layout, looseStone, { 0.42F, 0.42F, 0.45F });

    GatherableNode fiberPlant;
    fiberPlant.name = "Fiber Plant";
    fiberPlant.transform.position = { -3.2F, 0.45F, 0.4F };
    fiberPlant.transform.scale = { 0.5F, 0.9F, 0.5F };
    fiberPlant.interactionRadius = 0.9F;
    fiberPlant.remainingGatherCount = 2;
    fiberPlant.dropTable.AddDrop("fiber", 5, 5);
    fiberPlant.debugMessage = "Gathered tough plant fiber.";
    AddGatherable(layout, fiberPlant, { 0.28F, 0.65F, 0.32F });

    GatherableNode crackedRock;
    crackedRock.name = "Cracked Rock";
    crackedRock.transform.position = { 4.8F, 0.55F, -3.0F };
    crackedRock.transform.scale = { 1.0F, 1.0F, 1.0F };
    crackedRock.interactionRadius = 1.1F;
    crackedRock.remainingGatherCount = 1;
    crackedRock.dropTable.AddDrop("stone", 8, 8);
    crackedRock.requiredToolId = "primitive_tool";
    crackedRock.debugMessage = "Chipped stone from the cracked rock.";
    AddGatherable(layout, crackedRock, { 0.30F, 0.32F, 0.36F });

    layout.realmWisp = EnemyAgent::CreateRealmWisp();
    layout.realmWisp.transform.position = { 1.2F, 0.7F, -5.0F };

    return layout;
}

} // namespace rw::game
