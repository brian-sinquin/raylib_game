#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <cmath>
#include "components.h"
#include "../utils/seecs.h"

// ECS Systems namespace
namespace seecs
{
    using namespace components;

    namespace systems
    {
        // Boid System - Updates boid movement and makes them follow the mouse
        namespace boid_system
        {
            inline void Update(seecs::ECS& ecs, float deltaTime)
            {
                auto view = ecs.View<Transform, Motion, Boid>();
                std::vector<Vector2> positions;
                std::vector<Vector2> velocities;
                std::vector<seecs::EntityID> ids;

                // Gather all boid positions and velocities
                view.ForEach([&](seecs::EntityID id, Transform& t, Motion& m, Boid&)
                {
                    positions.push_back(t.position);
                    velocities.push_back(m.velocity);
                    ids.push_back(id);
                });

                // For each boid, calculate steering forces
                for (size_t i = 0; i < ids.size(); ++i)
                {
                    Vector2 pos = positions[i];
                    Vector2 vel = velocities[i];
                    Boid* boid = nullptr;
                    Transform* t = nullptr;
                    Motion* m = nullptr;

                    view.ForEach([&](seecs::EntityID id, Transform& t_, Motion& m_, Boid& b_)
                    {
                        if (id == ids[i]) { t = &t_; m = &m_; boid = &b_; }
                    });

                    if (!boid || !t || !m) continue;

                    Vector2 mouse = {(float)GetMouseX(), (float)GetMouseY()};
                    Vector2 steerToMouse = {mouse.x - pos.x, mouse.y - pos.y}; // Manual Vector2Subtract
                    float distToMouse = sqrtf(steerToMouse.x * steerToMouse.x + steerToMouse.y * steerToMouse.y); // Manual Vector2Length

                    if (distToMouse > 1.0f)
                    {
                        // Manual Vector2Normalize and Vector2Scale
                        float length = distToMouse;
                        steerToMouse.x = (steerToMouse.x / length) * boid->maxSpeed;
                        steerToMouse.y = (steerToMouse.y / length) * boid->maxSpeed;
                        steerToMouse.x -= vel.x;
                        steerToMouse.y -= vel.y; // Manual Vector2Subtract

                        float steerLength = sqrtf(steerToMouse.x * steerToMouse.x + steerToMouse.y * steerToMouse.y);
                        if (steerLength > boid->maxForce)
                        {
                            steerToMouse.x = (steerToMouse.x / steerLength) * boid->maxForce;
                            steerToMouse.y = (steerToMouse.y / steerLength) * boid->maxForce;
                        }
                    }
                    else
                    {
                        steerToMouse = {0, 0};
                    }

                    // Separation, Alignment, Cohesion
                    Vector2 sep = {0, 0};
                    Vector2 ali = {0, 0};
                    Vector2 coh = {0, 0};
                    int sepCount = 0, aliCount = 0, cohCount = 0;

                    for (size_t j = 0; j < ids.size(); ++j)
                    {
                        if (i == j) continue;

                        Vector2 diff = {pos.x - positions[j].x, pos.y - positions[j].y};
                        float d = sqrtf(diff.x * diff.x + diff.y * diff.y); // Manual Vector2Distance

                        if (d < boid->separationRadius && d > 0)
                        {
                            diff.x = (diff.x / d) * (1.0f / d); // Manual Vector2Normalize and Vector2Scale
                            diff.y = (diff.y / d) * (1.0f / d);
                            sep.x += diff.x; // Manual Vector2Add
                            sep.y += diff.y;
                            sepCount++;
                        }

                        if (d < boid->neighborRadius)
                        {
                            ali.x += velocities[j].x; // Manual Vector2Add
                            ali.y += velocities[j].y;
                            coh.x += positions[j].x; // Manual Vector2Add
                            coh.y += positions[j].y;
                            aliCount++;
                            cohCount++;
                        }
                    }

                    if (sepCount > 0)
                    {
                        sep.x /= sepCount; // Manual Vector2Scale
                        sep.y /= sepCount;
                    }

                    if (aliCount > 0)
                    {
                        ali.x /= aliCount; // Manual Vector2Scale
                        ali.y /= aliCount;
                    }

                    if (cohCount > 0)
                    {
                        coh.x /= cohCount; // Manual Vector2Scale
                        coh.y /= cohCount;
                        coh.x -= pos.x; // Manual Vector2Subtract
                        coh.y -= pos.y;
                    }

                    // Steering for alignment and cohesion
                    float aliLength = sqrtf(ali.x * ali.x + ali.y * ali.y);
                    if (aliLength > 0)
                    {
                        ali.x = (ali.x / aliLength) * boid->maxSpeed; // Manual Vector2Normalize and Vector2Scale
                        ali.y = (ali.y / aliLength) * boid->maxSpeed;
                        ali.x -= vel.x; // Manual Vector2Subtract
                        ali.y -= vel.y;

                        float aliSteerLength = sqrtf(ali.x * ali.x + ali.y * ali.y);
                        if (aliSteerLength > boid->maxForce)
                        {
                            ali.x = (ali.x / aliSteerLength) * boid->maxForce;
                            ali.y = (ali.y / aliSteerLength) * boid->maxForce;
                        }
                    }

                    float cohLength = sqrtf(coh.x * coh.x + coh.y * coh.y);
                    if (cohLength > 0)
                    {
                        coh.x = (coh.x / cohLength) * boid->maxSpeed; // Manual Vector2Normalize and Vector2Scale
                        coh.y = (coh.y / cohLength) * boid->maxSpeed;
                        coh.x -= vel.x; // Manual Vector2Subtract
                        coh.y -= vel.y;

                        float cohSteerLength = sqrtf(coh.x * coh.x + coh.y * coh.y);
                        if (cohSteerLength > boid->maxForce)
                        {
                            coh.x = (coh.x / cohSteerLength) * boid->maxForce;
                            coh.y = (coh.y / cohSteerLength) * boid->maxForce;
                        }
                    }

                    float sepLength = sqrtf(sep.x * sep.x + sep.y * sep.y);
                    if (sepLength > 0)
                    {
                        sep.x = (sep.x / sepLength) * boid->maxSpeed; // Manual Vector2Normalize and Vector2Scale
                        sep.y = (sep.y / sepLength) * boid->maxSpeed;
                        sep.x -= vel.x; // Manual Vector2Subtract
                        sep.y -= vel.y;

                        float sepSteerLength = sqrtf(sep.x * sep.x + sep.y * sep.y);
                        if (sepSteerLength > boid->maxForce)
                        {
                            sep.x = (sep.x / sepSteerLength) * boid->maxForce;
                            sep.y = (sep.y / sepSteerLength) * boid->maxForce;
                        }
                    }

                    // Weights for each behavior
                    float sepWeight = 1.5f, aliWeight = 1.0f, cohWeight = 1.0f, mouseWeight = 1.2f;
                    Vector2 accel = {0, 0};
                    accel.x += sep.x * sepWeight; // Manual Vector2Add and Vector2Scale
                    accel.y += sep.y * sepWeight;
                    accel.x += ali.x * aliWeight;
                    accel.y += ali.y * aliWeight;
                    accel.x += coh.x * cohWeight;
                    accel.y += coh.y * cohWeight;
                    accel.x += steerToMouse.x * mouseWeight;
                    accel.y += steerToMouse.y * mouseWeight;

                    m->acceleration = accel;

                    // Clamp velocity
                    m->velocity.x += m->acceleration.x * deltaTime; // Manual Vector2Add and Vector2Scale
                    m->velocity.y += m->acceleration.y * deltaTime;

                    float velLength = sqrtf(m->velocity.x * m->velocity.x + m->velocity.y * m->velocity.y);
                    if (velLength > boid->maxSpeed)
                    {
                        m->velocity.x = (m->velocity.x / velLength) * boid->maxSpeed; // Manual Vector2Normalize and Vector2Scale
                        m->velocity.y = (m->velocity.y / velLength) * boid->maxSpeed;
                    }
                }
            }
        }

        // Movement System - Updates position based on velocity and acceleration
        namespace movement_system {
            inline void Update(seecs::ECS& ecs, float deltaTime) {
                auto view = ecs.View<Transform, Motion>();
                view.ForEach([&](seecs::EntityID id, Transform& transform, Motion& motion) {
                    // Update velocity based on acceleration
                    motion.velocity.x += motion.acceleration.x * deltaTime;
                    motion.velocity.y += motion.acceleration.y * deltaTime;

                    // Update position based on velocity
                    transform.position.x += motion.velocity.x * deltaTime;
                    transform.position.y += motion.velocity.y * deltaTime;
                });
            }
        }

        // Render System - Draws sprites and boids
        namespace render_system {
            inline void Update(seecs::ECS& ecs) {
                // Render sprites (for any remaining sprite entities)
                auto spriteView = ecs.View<Transform, Sprite>();
                spriteView.ForEach([&](seecs::EntityID id, Transform& transform, Sprite& sprite) {
                    if (sprite.texture.id == 0) return; // Skip if no texture

                    Rectangle destRect = {
                        transform.position.x,
                        transform.position.y,
                        sprite.sourceRect.width * transform.scale.x,
                        sprite.sourceRect.height * transform.scale.y
                    };

                    // Apply flipping
                    int flipX = sprite.flipX ? -1 : 1;
                    int flipY = sprite.flipY ? -1 : 1;

                    DrawTexturePro(
                        sprite.texture,
                        sprite.sourceRect,
                        destRect,
                        {destRect.width / 2.0f, destRect.height / 2.0f}, // origin at center
                        transform.rotation,
                        sprite.tint
                    );
                });

                // Render boids as triangles
                auto boidView = ecs.View<Transform, Motion, Boid>();
                boidView.ForEach([&](seecs::EntityID id, Transform& transform, Motion& motion, Boid&)
                {
                    // Calculate triangle points based on velocity direction
                    Vector2 direction = {motion.velocity.x, motion.velocity.y};
                    float velLength = sqrtf(direction.x * direction.x + direction.y * direction.y);

                    if (velLength < 0.1f)
                    {
                        direction = {0, -1}; // Default upward if not moving
                    }
                    else
                    {
                        direction.x /= velLength; // Manual Vector2Normalize
                        direction.y /= velLength;
                    }

                    float size = 10.0f;
                    Vector2 tip = transform.position;
                    Vector2 left = {
                        transform.position.x - direction.x * size - direction.y * size * 0.5f,
                        transform.position.y - direction.y * size + direction.x * size * 0.5f
                    };
                    Vector2 right = {
                        transform.position.x - direction.x * size + direction.y * size * 0.5f,
                        transform.position.y - direction.y * size - direction.x * size * 0.5f
                    };

                    // Draw triangle
                    DrawTriangle(tip, left, right, BLACK);
                    DrawTriangleLines(tip, left, right, RED);
                });
            }
        }

        // Collision System - Detects and handles collisions
        namespace collision_system {
            inline void Update(seecs::ECS& ecs) {
                auto view = ecs.View<Transform, Collider>();
                auto entities = view.GetPacked();

                // Check collisions between all pairs of entities
                for (size_t i = 0; i < entities.size(); ++i) {
                    for (size_t j = i + 1; j < entities.size(); ++j) {
                        auto& [id1, components1] = entities[i];
                        auto& [id2, components2] = entities[j];

                        auto& [transform1, collider1] = components1;
                        auto& [transform2, collider2] = components2;

                        Rectangle rect1 = {
                            transform1.position.x - collider1.bounds.width / 2.0f,
                            transform1.position.y - collider1.bounds.height / 2.0f,
                            collider1.bounds.width,
                            collider1.bounds.height
                        };

                        Rectangle rect2 = {
                            transform2.position.x - collider2.bounds.width / 2.0f,
                            transform2.position.y - collider2.bounds.height / 2.0f,
                            collider2.bounds.width,
                            collider2.bounds.height
                        };

                        if (CheckCollisionRecs(rect1, rect2)) {
                            // Handle collision - for now just print
                            // In a real game, you'd dispatch collision events
                            std::cout << "Collision detected between entities " << id1 << " and " << id2 << std::endl;
                        }
                    }
                }
            }
        }

        // Health System - Manages health regeneration and death
        namespace health_system {
            inline void Update(seecs::ECS& ecs, float deltaTime) {
                auto view = ecs.View<Health>();
                view.ForEach([&](seecs::EntityID id, Health& health) {
                    // Simple health regeneration (1 HP per second)
                    if (health.current < health.max) {
                        health.current = std::min(health.max, health.current + (int)(deltaTime * 10.0f));
                    }

                    // Check for death
                    if (health.current <= 0) {
                        // In a real game, you'd mark entity for deletion
                        std::cout << "Entity died!" << std::endl;
                    }
                });
            }
        }

        // Player Input System - Handles player-controlled entities
        namespace player_input_system {
            inline void Update(seecs::ECS& ecs, float deltaTime) {
                auto view = ecs.View<Transform, Motion, PlayerControlled>();
                view.ForEach([&](seecs::EntityID id, Transform& transform, Motion& motion, PlayerControlled&) {
                    // Reset acceleration
                    motion.acceleration = {0.0f, 0.0f};

                    // Handle input
                    float speed = 200.0f; // pixels per second

                    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                        motion.acceleration.y = -speed;
                    }
                    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                        motion.acceleration.y = speed;
                    }
                    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                        motion.acceleration.x = -speed;
                    }
                    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                        motion.acceleration.x = speed;
                    }

                    // Apply friction when no input
                    if (motion.acceleration.x == 0.0f && motion.acceleration.y == 0.0f) {
                        motion.velocity.x *= 0.99f;
                        motion.velocity.y *= 0.99f;
                    }
                });
            }
        }

        // AI System - Simple AI for AI-controlled entities
        namespace ai_system {
            inline void Update(seecs::ECS& ecs, float deltaTime) {
                auto view = ecs.View<Transform, Motion, AIControlled>();
                view.ForEach([&](seecs::EntityID id, Transform& transform, Motion& motion, AIControlled&) {
                    // Simple wandering AI
                    static float aiTimer = 0.0f;
                    aiTimer += deltaTime;

                    if (aiTimer > 2.0f) { // Change direction every 2 seconds
                        aiTimer = 0.0f;
                        float angle = GetRandomValue(0, 360) * DEG2RAD;
                        motion.acceleration.x = cos(angle) * 100.0f;
                        motion.acceleration.y = sin(angle) * 100.0f;
                    }

                    // Apply friction
                    motion.velocity.x *= 0.95f;
                    motion.velocity.y *= 0.95f;
                });
            }
        }

        // System Manager - Orchestrates all systems
        class SystemManager {
        private:
            seecs::ECS& m_ecs;

        public:
            SystemManager(seecs::ECS& ecs) : m_ecs(ecs) {}

            void Update(float deltaTime) {
                // Update systems in order
                movement_system::Update(m_ecs, deltaTime);
                boid_system::Update(m_ecs, deltaTime);
                collision_system::Update(m_ecs);
                health_system::Update(m_ecs, deltaTime);
            }

            void Render() {
                render_system::Update(m_ecs);
            }
        };
    }
}
