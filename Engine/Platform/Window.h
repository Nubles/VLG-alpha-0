#pragma once

#include "Engine/Input/InputState.h"

#include <memory>
#include <string>

namespace rw::platform {

struct WindowConfig {
    std::string title = "Realmbound Wilds";
    int width = 1280;
    int height = 720;
};

class Window {
public:
    struct Impl;

    explicit Window(const WindowConfig& config);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept;
    Window& operator=(Window&&) noexcept;

    bool IsOpen() const;
    void PollEvents(rw::input::InputState& input);
    void ClearBlankScreen();
    void SetTitle(const std::string& title);

private:
    std::unique_ptr<Impl> m_impl;
};

} // namespace rw::platform
