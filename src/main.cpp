#include "raylib.h"
#include "core/game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// Static game instance for WebAssembly
static Game* gameInstance = nullptr;

// Static function for Emscripten main loop
static void MainLoop()
{
    if (gameInstance)
    {
        gameInstance->Run();
    }
}

int main(void)
{
    Game game;
    gameInstance = &game;

    // Initialize the game
    if (!game.Initialize())
    {
        return EXIT_FAILURE;
    }

    // Run the game loop
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(MainLoop, 60, 1);
    #else
        game.Run();
    #endif

    // Cleanup
    game.Shutdown();

    return EXIT_SUCCESS;
}
