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
    if (isDown) {
        RequestQuit();
    }
}

bool InputState::EscapeDown() const
{
    return m_escapeDown;
}

} // namespace rw::input

