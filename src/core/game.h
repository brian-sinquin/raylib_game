#pragma once

#include "../global.h"

/**
 * @brief Main Game class that manages the application lifecycle
 *
 * This class handles initialization, the main game loop, and cleanup.
 * It uses an ECS (Entity Component System) architecture for game logic.
 */
class Game
{
private:
    // ECS instance and system manager
    seecs::ECS ecs;
    seecs::systems::SystemManager* systemManager;

    // Fixed timestep timing
    const double FIXED_DT = 1.0 / 60.0;
    double accumulator = 0.0;

public:
    Game();
    ~Game();

    /**
     * @brief Initialize the game and its systems
     * @return true if initialization successful, false otherwise
     */
    bool Initialize();

    /**
     * @brief Run the main game loop
     */
    void Run();

    /**
     * @brief Cleanup and shutdown the game
     */
    void Shutdown();

private:
    /**
     * @brief Update game logic with fixed timestep
     * @param dt Delta time for this update
     */
    void UpdateFixed(float dt);

    /**
     * @brief Render the current frame
     */
    void DrawFrame();

    /**
     * @brief Set up the boids example with entities and components
     */
    void SetupBoidsExample();
};

