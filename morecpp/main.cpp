#include <iostream>
#include <random>
#include <memory>
#include "road.h"
#include "car.h"
#include "gameloop.h"

int main() {
    // Create a random number generator for varied speeds
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> speedDist(3.0f, 8.0f);

    // Create road (position, length, width, lanes)
    Road road(Vector3(0, 0, 0), 120.0f, 12.0f, 3);

    // Create cars with different speeds using shared pointers
    auto car1 = std::make_shared<Car>(
        Vector3(0, road.getLanePosition(0), 0),   // Position in lane 0
        Vector3(2, 1, 1),                         // Dimensions
        Color::Red(),                             // Color
        speedDist(gen)                            // Random preferred speed
    );

    auto car2 = std::make_shared<Car>(
        Vector3(10, road.getLanePosition(1), 0),  // Position in lane 1 (with offset)
        Vector3(2, 1, 1),                         // Dimensions
        Color::Blue(),                            // Color
        speedDist(gen)                            // Random preferred speed
    );

    auto car3 = std::make_shared<Car>(
        Vector3(20, road.getLanePosition(2), 0),  // Position in lane 2 (with offset)
        Vector3(2, 1, 1),                         // Dimensions
        Color::Green(),                           // Color
        speedDist(gen)                            // Random preferred speed
    );

    auto car4 = std::make_shared<Car>(
        Vector3(25, road.getLanePosition(2), 0),  // Position in lane 2 (with offset)
        Vector3(2, 1, 1),                         // Dimensions
        Color::Green(),                           // Color
        speedDist(gen)                            // Random preferred speed
    );

    // Add cars to road
    road.addCar(car1);
    road.addCar(car2);
    road.addCar(car3);
    road.addCar(car4);

    // Run game loop
    GameLoop::run(road);

    return 0;
}