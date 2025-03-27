#pragma once
#include "road.h"
#include <iostream>

class ConsoleDisplay {
public:
    static void render(const Road& road) {
        // Road length representation
        const float roadLength = road.getLength();
        const std::vector<Car>& cars = road.getCars();

        std::cout << "Road Length: " << roadLength << std::endl;

        for (const auto& car : cars) {
            // Create road representation
            std::string roadDisplay(static_cast<int>(roadLength), '.');

            // Get car's x position
            int carPosition = static_cast<int>(car.getPosition().x);

            // Place car if within road bounds
            if (carPosition >= 0 && carPosition < roadLength) {
                roadDisplay[carPosition] = 'C';
            }

            // Color-based representation (simplified)
            Color carColor = car.getColor();
            if (carColor.r > 0.5f) std::cout << "Red ";
            else if (carColor.b > 0.5f) std::cout << "Blue ";
            else if (carColor.g > 0.5f) std::cout << "Green ";

            //std::cout << "Car Position: " << car.getPosition().x << std::endl;
            std::cout << roadDisplay << std::endl;
        }
    }
};