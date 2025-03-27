#include <iostream>
#include "road.h"
#include "gameloop.h"

int main() {
    // Create road (position, length, width, lanes)
    Road road(Vector3(0, 0, 0), 50.0f, 10.0f, 2);

    // Create cars (position, dimensions, color)
    Car car1(Vector3(0, 2, 0), Vector3(2, 1, 1), Color::Red());
    Car car2(Vector3(0, 4, 0), Vector3(2, 1, 1), Color::Blue());
    Car car3(Vector3(0, 6, 0), Vector3(2, 1, 1), Color::Green());

    // Add cars to road
    road.addCar(car1);
    road.addCar(car2);
    road.addCar(car3);

    // Run game loop
    GameLoop::run(road);

    return 0;
}