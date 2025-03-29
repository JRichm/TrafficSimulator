#include "road.h"

void Road::addCar(std::shared_ptr<Car> car) {
    car->setRoad(this);
    cars.push_back(car);
}

void Road::update(float deltaTime) {
    // First gather data about neighboring cars for each car
    for (auto& car : cars) {
        std::vector<std::shared_ptr<Car>> nearbyCars;

        // Find cars within a certain range
        for (auto& otherCar : cars) {
            if (car != otherCar) {
                // Calculate forward distance between cars
                float xDistance = otherCar->getPosition().x - car->getPosition().x;

                // Only consider cars ahead within a reasonable distance
                if (xDistance > 0 && xDistance < 30.0f) {
                    nearbyCars.push_back(otherCar);
                }
            }
        }

        // Adjust speed based on traffic
        car->adjustSpeedForTraffic(nearbyCars, deltaTime);

        // Check if car should change lanes
        if (car->getLaneChangeTimer() > car->getMinLaneChangeTime() && car->shouldChangeLane(nearbyCars)) {
            // Determine which direction to change (simplified for now)
            int currentLane = car->getCurrentLane();

            if (currentLane > 0 && currentLane < laneCount - 1) {
                // Random lane change direction for middle lanes
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, 1);
                car->changeLane((dist(gen) * 2) - 1); // -1 or 1
            } else if (currentLane == 0) {
                // Right lane change only for leftmost lane
                car->changeLane(1);
            } else {
                // Left lane change only for rightmost lane
                car->changeLane(-1);
            }

            car->resetLaneChangeTimer();
        }
    }

    // Then update all cars
    for (auto& car : cars) {
        car->update(deltaTime);
    }

    // Remove cars that have gone beyond the road length
    cars.erase(
        std::remove_if(cars.begin(), cars.end(),
            [this](const std::shared_ptr<Car>& car) {
                return car->getPosition().x >= length;
            }
        ),
        cars.end()
    );
}

float Road::getLanePosition(int lane) const {
    return position.y + (lane + 0.5f) * laneWidth;
}