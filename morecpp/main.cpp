#include <iostream>
#include <random>
#include "road.h"
#include "car.h"
#include "openglDisplay.h" // This already includes GLAD

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
        Color::Yellow(),                          // Color
        speedDist(gen)                            // Random preferred speed
    );

    // Add cars to road
    road.addCar(car1);
    road.addCar(car2);
    road.addCar(car3);
    road.addCar(car4);

    // Add more cars (optional)
    for (int i = 0; i < 5; i++) {
        int lane = gen() % 3;  // Random lane
        float offset = 35.0f + i * 10.0f;  // Spread cars out

        auto car = std::make_shared<Car>(
            Vector3(offset, road.getLanePosition(lane), 0),
            Vector3(2, 1, 1),
            Color(gen() % 200 + 55, gen() % 200 + 55, gen() % 200 + 55),  // Random color
            speedDist(gen)
        );

        road.addCar(car);
    }

    // Run the simulation with OpenGL rendering
    OpenGLGameLoop::run(road);

    return 0;
}