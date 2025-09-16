#include "raylib.h"
#include "core/game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int main(void)
{
    Game game;

    // Initialize the game
    if (!game.Initialize())
    {
        return EXIT_FAILURE;
    }

    // Run the game loop
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop([]() { game.Run(); }, 0, 1);
    #else
        game.Run();
    #endif

    // Cleanup
    game.Shutdown();

    return EXIT_SUCCESS;
}
