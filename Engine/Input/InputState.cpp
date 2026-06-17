#include "Engine/Input/InputState.h"

namespace rw::input {

void InputState::BeginFrame()
{
    m_escapeDown = false;
}

void InputState::RequestQuit()
{
    m_quitRequested = true;
}

bool InputState::QuitRequested() const
{
    return m_quitRequested;
}

void InputState::SetEscapeDown(bool isDown)
{
    m_escapeDown = isDown;
    SetKeyDown(Key::Escape, isDown);
    if (isDown) {
        RequestQuit();
    }
}

bool InputState::EscapeDown() const
{
    return m_escapeDown;
}

void InputState::SetKeyDown(Key key, bool isDown)
{
    m_keys[static_cast<int>(key)] = isDown;
}

bool InputState::IsKeyDown(Key key) const
{
    return m_keys[static_cast<int>(key)];
}

} // namespace rw::input
