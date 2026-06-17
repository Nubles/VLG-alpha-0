#include "Engine/Core/Application.h"
#include "Game/Source/SandboxGame.h"

int main()
{
    rw::core::Application app;
    rw::game::SandboxGame game;
    return app.Run(game);
}
