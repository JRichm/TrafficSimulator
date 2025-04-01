#include "car.h"
#include "road.h"

void Car::setRoad(Road* r) {
    road = r;
}

void Car::update(float deltaTime) {
    // Update lane change timer
    laneChangeTimer += deltaTime;

    // Create velocity vector based on current speed
    velocity = Vector3(currentSpeed, 0, 0);

    // Move car based on velocity and delta time
    position = position + (velocity * deltaTime);
}

int Car::getCurrentLane() const {
    if (!road) return 0;

    // Calculate lane based on y position
    float relativeY = position.y - road->getPosition().y;
    return static_cast<int>(relativeY / road->getLaneWidth());
}

void Car::changeLane(int direction) {
    if (!road) return;

    int currentLane = getCurrentLane();
    int targetLane = currentLane + direction;

    // Check bounds
    if (targetLane >= 0 && targetLane < road->getLaneCount()) {
        // Set position to new lane
        position.y = road->getLanePosition(targetLane);
    }
}

void Car::adjustSpeedForTraffic(const std::vector<std::shared_ptr<Car>>& nearbyCars, float deltaTime) {
    if (nearbyCars.empty()) {
        // No cars ahead, gradually accelerate to preferred speed
        currentSpeed = std::min(currentSpeed + 2.0f * deltaTime, preferredSpeed);
        return;
    }

    // Find the closest car ahead in the same lane
    float closestDistance = std::numeric_limits<float>::max();
    std::shared_ptr<Car> carAhead = nullptr;

    for (auto& otherCar : nearbyCars) {
        if (getCurrentLane() == otherCar->getCurrentLane()) {
            float distance = otherCar->getPosition().x - position.x;
            if (distance < closestDistance) {
                closestDistance = distance;
                carAhead = otherCar;
            }
        }
    }

    if (carAhead) {
        // Maintain safe distance (simple model)
        float safeDistance = currentSpeed * 2.0f; // 2 seconds rule

        if (closestDistance < safeDistance) {
            // Too close - slow down
            currentSpeed = std::max(currentSpeed - 5.0f * deltaTime, 0.0f);
        } else if (closestDistance > safeDistance * 1.5f) {
            // Far enough - can accelerate if below preferred speed
            currentSpeed = std::min(currentSpeed + 2.0f * deltaTime,
                std::min(preferredSpeed, carAhead->getCurrentSpeed()));
        }
    }
}

bool Car::shouldChangeLane(const std::vector<std::shared_ptr<Car>>& nearbyCars) {
    // Simple lane-changing logic:
    // Change lanes if there's a car ahead slowing us down

    // Check if we're being slowed down
    if (currentSpeed < preferredSpeed * 0.8f) {
        // Check if there's a car ahead in our lane
        for (auto& otherCar : nearbyCars) {
            if (getCurrentLane() == otherCar->getCurrentLane()) {
                float distance = otherCar->getPosition().x - position.x;
                if (distance < currentSpeed * 3.0f) {
                    return true; // Car ahead is slowing us down
                }
            }
        }
    }

    return false;
}