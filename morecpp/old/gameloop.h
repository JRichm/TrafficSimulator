#pragma once
#include <chrono>
#include <thread>
#include "road.h"
#include "consoledisplay.h"

class GameLoop {
public:
    static void run(Road& road, int iterations = 100, int msDelay = 200) {
        float deltaTime = msDelay / 1000.0f;  // Convert ms to seconds for physics

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

            // Check if all cars are off the road
            bool allCarsFinished = true;
            for (const auto& car : road.getCars()) {
                if (car->getPosition().x < road.getLength()) {
                    allCarsFinished = false;
                    break;
                }
            }

            if (allCarsFinished) {
                std::cout << "All cars have completed the road." << std::endl;
                break;
            }
        }
    }
};