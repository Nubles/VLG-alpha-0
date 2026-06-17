#include "Engine/Core/Application.h"

#include "Engine/Core/Logger.h"

#include <chrono>
#include <exception>
#include <thread>

namespace rw::core {

int Application::Run(GameLayer& game)
{
    try {
        Logger::Info("Starting Realmbound Wilds engine bootstrap.");

        rw::platform::Window window({
            "Realmbound Wilds - Basic 3D Sandbox",
            1280,
            720,
        });
        m_renderer.Initialize(window);

        while (window.IsOpen() && !m_input.QuitRequested()) {
            m_input.BeginFrame();
            const float deltaSeconds = m_clock.Tick();

            window.PollEvents(m_input);
            game.OnUpdate(deltaSeconds, m_input);
            game.OnRender(m_renderer, window);
            m_debugOverlay.Update(window, deltaSeconds);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        m_renderer.Shutdown();
        Logger::Info("Engine shutdown completed.");
        return 0;
    } catch (const std::exception& exception) {
        Logger::Error(exception.what());
        return 1;
    }
}

} // namespace rw::core
