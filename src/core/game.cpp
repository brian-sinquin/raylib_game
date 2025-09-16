#include "game.h"
#include <sstream>

// Game Implementation
Game::Game() : systemManager(nullptr)
{
    // Constructor - initialize member variables
}

Game::~Game()
{
    Shutdown();
}

bool Game::Initialize()
{
    // Create window title with version
    std::stringstream ss;
    ss << GAME_TITLE << " v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH;
    std::string windowTitle = ss.str();

    // Initialize raylib
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, windowTitle.c_str());
    SetExitKey(KEY_NULL); // Disable default exit key (ESC)
    InitAudioDevice();
    SetTargetFPS(TARGET_FPS);

    // Seed random number generator
    srand((unsigned)time(NULL));

    // Initialize ECS and systems
    systemManager = new seecs::systems::SystemManager(ecs);

    // Components will be registered automatically when first used
    // No need to call RegisterComponent() explicitly

    // Set up the boids example
    SetupBoidsExample();

    return true;
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        double dt = GetFrameTime();
        accumulator += dt;

        // Fixed timestep updates
        while (accumulator >= FIXED_DT)
        {
            UpdateFixed(FIXED_DT);
            accumulator -= FIXED_DT;
        }

        DrawFrame();
    }
}

void Game::Shutdown()
{
    if (systemManager)
    {
        delete systemManager;
        systemManager = nullptr;
    }

    CloseAudioDevice();
    CloseWindow();
}

void Game::UpdateFixed(float dt)
{
    if (systemManager)
    {
        systemManager->Update(dt);
    }
}

void Game::SetupBoidsExample()
{
    // Create boids with random positions and velocities
    const int NUM_BOIDS = 100;

    for (int i = 0; i < NUM_BOIDS; ++i)
    {
        auto boid = ecs.CreateEntity(std::string("Boid_") + std::to_string(i));

        // Random position within screen bounds
        float x = (float)GetRandomValue(50, DEFAULT_WINDOW_WIDTH - 50);
        float y = (float)GetRandomValue(50, DEFAULT_WINDOW_HEIGHT - 50);

        // Random initial velocity
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        float speed = (float)GetRandomValue(20, 80);
        float vx = cos(angle) * speed;
        float vy = sin(angle) * speed;

        ecs.Add<seecs::components::Transform>(boid, {{x, y}, 0.0f, {1.0f, 1.0f}});
        ecs.Add<seecs::components::Motion>(boid, {{vx, vy}, {0.0f, 0.0f}});
        ecs.Add<seecs::components::Boid>(boid);
        ecs.Add<seecs::components::Name>(boid, {std::string("Boid_") + std::to_string(i)});
    }

    std::cout << "Boids Example Setup Complete!" << std::endl;
    std::cout << "Created " << NUM_BOIDS << " boids" << std::endl;
    std::cout << "Move your mouse to guide the boids!" << std::endl;
}void Game::DrawFrame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Render ECS entities
    if (systemManager)
    {
        systemManager->Render();
    }

    DRAW_FPS;
    DRAW_MOUSE_POS;
    EndDrawing();
}