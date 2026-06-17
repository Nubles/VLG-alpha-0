#include "Engine/Core/Application.h"

#include "Engine/Core/Logger.h"

#include <chrono>
#include <exception>
#include <thread>

namespace rw::core {

int Application::Run()
{
    try {
        Logger::Info("Starting Realmbound Wilds engine bootstrap.");

        rw::platform::Window window({
            "Realmbound Wilds - Engine Bootstrap",
            1280,
            720,
        });

        while (window.IsOpen() && !m_input.QuitRequested()) {
            m_input.BeginFrame();
            const float deltaSeconds = m_clock.Tick();

            window.PollEvents(m_input);
            m_renderer.BeginFrame(window);
            m_debugOverlay.Update(window, deltaSeconds);
            m_renderer.EndFrame();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        Logger::Info("Engine shutdown completed.");
        return 0;
    } catch (const std::exception& exception) {
        Logger::Error(exception.what());
        return 1;
    }
}

} // namespace rw::core
