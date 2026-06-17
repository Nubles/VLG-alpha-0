#pragma once

#include "Engine/Input/InputState.h"
#include "Engine/Scene/Camera.h"
#include "Game/Source/PlayerVitals.h"

namespace rw::game {

class PlayerController {
public:
    PlayerController();

    void Update(float deltaSeconds, const rw::input::InputState& input);

    const rw::scene::Camera& Camera() const;
    const PlayerVitals& Vitals() const;

private:
    rw::scene::Camera m_camera;
    PlayerVitals m_vitals;
};

} // namespace rw::game
