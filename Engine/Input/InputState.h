#pragma once

namespace rw::input {

class InputState {
public:
    void BeginFrame();

    void RequestQuit();
    bool QuitRequested() const;

    void SetEscapeDown(bool isDown);
    bool EscapeDown() const;

private:
    bool m_quitRequested = false;
    bool m_escapeDown = false;
};

} // namespace rw::input

