#include "Engine/Scene/Scene.h"

#include <utility>

namespace rw::scene {

void Scene::AddObject(SceneObject object)
{
    m_objects.push_back(std::move(object));
}

const std::vector<SceneObject>& Scene::Objects() const
{
    return m_objects;
}

} // namespace rw::scene
