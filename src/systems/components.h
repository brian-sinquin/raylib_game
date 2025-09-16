#pragma once

#include "raylib.h"
#include <string>
#include <vector>

// Forward declaration of ECS
namespace seecs
{
    namespace components
    {
        // Transform component for position, rotation, and scale
        struct Transform
        {
            Vector2 position = {0.0f, 0.0f};
            float rotation = 0.0f; // in degrees
            Vector2 scale = {1.0f, 1.0f};
        };

        // Motion component for velocity and acceleration
        struct Motion
        {
            Vector2 velocity = {0.0f, 0.0f};
            Vector2 acceleration = {0.0f, 0.0f};
        };

        // Boid component for boid simulation
        struct Boid
        {
            float maxSpeed = 400.0f;
            float maxForce = 100.0f;
            float neighborRadius = 40.0f;
            float separationRadius = 20.0f;
        };

        // Sprite component for rendering textures
        struct Sprite
        {
            Texture2D texture = {0};
            Rectangle sourceRect = {0, 0, 0, 0};
            Color tint = WHITE;
            bool flipX = false;
            bool flipY = false;
        };

        // Collider component for collision detection
        struct Collider
        {
            Rectangle bounds = {0, 0, 0, 0};
            bool isTrigger = false;
        };

        // Health component for damage/health system
        struct Health
        {
            int current = 100;
            int max = 100;
        };

        // Marker component for player-controlled entities
        struct PlayerControlled
        {
            // Empty marker component
        };

        // Marker component for AI-controlled entities
        struct AIControlled
        {
            // Empty marker component
        };

        // Name component for entity identification
        struct Name
        {
            std::string value;
        };





    }
}