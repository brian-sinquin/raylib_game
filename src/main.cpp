

#include "raylib.h"
#include "CONSTANTS.h"
#include <stdio.h>
#include <string>
#include "utils/json.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;



//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------


static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization

    //---------------------------------------------------------
        std::string version = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH);
        std::string windowTitle = "raylib game - v" + version;
        InitWindow(screenWidth, screenHeight, windowTitle.c_str());
       
        InitAudioDevice();      // Initialize audio device


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

  


    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
   
    
    BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, RED);



    EndDrawing();
    //----------------------------------------------------------------------------------
}
