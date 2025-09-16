#pragma once

// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <format>

// Project includes
#include "core/constants.h"
#include "raylib.h"

// ECS and game systems
#include "utils/seecs.h"
#include "systems/components.h"
#include "systems/systems.h"

// Debug configuration
#define DEBUG_MODE 1

// Debug drawing macros
#if DEBUG_MODE
    #define DRAW_FPS DrawText(std::format("FPS: {}", GetFPS()).c_str(), 10, 10, 20, RED)
    #define DRAW_MOUSE_POS DrawText(std::format("Mouse: ({}, {})", (int)GetMouseX(), (int)GetMouseY()).c_str(), 10, 30, 20, RED)
#else
    #define DRAW_FPS
    #define DRAW_MOUSE_POS
#endif