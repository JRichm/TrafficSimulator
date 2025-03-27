#pragma once
#include <chrono>
#include <thread>
#include "road.h"
#include "consoledisplay.h"

class GameLoop {
public:
    static void run(Road& road, int iterations = 100, int msDelay = 200) {
        float deltaTime = 0.2f;  // Fixed time step

        for (int i = 0; i < iterations; ++i) {
            // Clear console
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            // Update game state
            road.update(deltaTime);

            // Render
            ConsoleDisplay::render(road);

            // Pause
            std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));
        }
    }
};