#include "consoledisplay.h"

void ConsoleDisplay::render(const Road& road) {
    // Road length representation
    const float roadLength = road.getLength();
    const int numLanes = road.getLaneCount();
    const std::vector<std::shared_ptr<Car>>& cars = road.getCars();

    std::cout << "Road Length: " << roadLength << ", Lanes: " << numLanes << std::endl;

    // Create a 2D vector to represent the road
    std::vector<std::string> roadDisplay(numLanes * 2 - 1, std::string(static_cast<int>(roadLength), ' '));

    // Add lane separators that span entire road length
    for (int i = 1; i < numLanes; ++i) {
        int separatorIndex = (i - 1) * 2 + 1;
        for (int j = 0; j < roadLength; ++j) {
            roadDisplay[separatorIndex][j] = '-';
        }
    }

    // Place cars in their respective lanes
    for (const auto& car : cars) {
        int carPosition = static_cast<int>(car->getPosition().x);
        int currentLane = car->getCurrentLane();

        // Calculate display lane (accounting for separator rows)
        int displayLane = currentLane * 2;

        // Place car if within road bounds
        if (carPosition >= 0 && carPosition < roadLength) {
            roadDisplay[displayLane][carPosition] = car->getIcon();
        }

        // Display car info
        std::cout << "Car at lane " << currentLane
            << ", position " << car->getPosition().x
            << ", speed " << car->getCurrentSpeed()
            << std::endl;
    }

    // Render the road
    for (const auto& lane : roadDisplay) {
        std::cout << lane << std::endl;
    }
}