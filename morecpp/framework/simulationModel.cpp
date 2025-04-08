#include "simulationModel.h"

#include <random>
#include <iostream>

#include "../road/intersection.h"


void SimulationModel::update(float deltaTime) {
	if (isPaused) return;

	float scaledTime = deltaTime * timeScale;
	simulationTime += scaledTime;

	roadNetwork.update(scaledTime);
}


void SimulationModel::buildCustomNetwork() {
    roadNetwork = RoadNetwork();

    // T-junction 
    std::cout << "Creating T-Junction..." << std::endl;
    Intersection::createTJunction(roadNetwork, "t_junction_center", Vector3(0, 0, 0), 400.0f, 32.0f, 10.0f);

    // 4 way intersection
    std::cout << "Creating 4 way intersection..." << std::endl;
    auto fourWayJunction = Intersection::createIntersectionWithRoads(
        IntersectionType::TRAFFIC_LIGHT,
        "four_way_junction",
        Vector3(400.0f, 400.0f, 0.0f),
        { 0, 90, 180, 270 },
        300.0f, 32.0f, 8.0f
    );

    if (fourWayJunction) {
        roadNetwork.addJunction(fourWayJunction);

        // add roads
        for (const auto& road : fourWayJunction->getConnectedRoads()) {
            roadNetwork.addRoadSegment(road);

            // add junctions
            if (auto endJunction = road->getEndJunction()) {
                if (endJunction->getId() != fourWayJunction->getId()) {
                    roadNetwork.addJunction(endJunction);
                }
            }
        }
    }

    // Y-junction
    std::cout << "Creating Y-Junction..." << std::endl;
    auto yJunction = Intersection::createIntersectionWithRoads(
        IntersectionType::SIMPLE,
        "y_junction",
        Vector3(-200.0f, 300.0f, 0.0f),
        { 0, 120, 240 },
        200.0f,
        32.0f,
        8.0f
    );

    if (yJunction) {
        roadNetwork.addJunction(yJunction);

        // Add roads
        for (const auto& road : yJunction->getConnectedRoads()) {
            roadNetwork.addRoadSegment(road);

            // Add junctions
            if (auto endJunction = road->getEndJunction()) {
                if (endJunction->getId() != yJunction->getId()) {
                    roadNetwork.addJunction(endJunction);
                }
            }
        }
    }

    // add destinations
    std::cout << "Adding destinations..." << std::endl;
    auto destination1 = std::make_shared<Destination>(Vector3(-400.0f, 0.0f, 0.0f), "West Destination", 15.0f);
    auto destination2 = std::make_shared<Destination>(Vector3(400.0f, 0.0f, 0.0f), "East Destination", 15.0f);
    auto destination3 = std::make_shared<Destination>(Vector3(0.0f, 400.0f, 0.0f), "North Destination", 15.0f);
    auto destination4 = std::make_shared<Destination>(Vector3(700.0f, 400.0f, 0.0f), "Northeast Destination", 15.0f);

    roadNetwork.addDestination(destination1);
    roadNetwork.addDestination(destination2);
    roadNetwork.addDestination(destination3);
    roadNetwork.addDestination(destination4);

    // add spawn points
    std::cout << "Adding spawn points..." << std::endl;

    auto roadSegments = roadNetwork.getAllRoadSegments();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> spawnRateDist(5.0f, 15.0f);

    for (const auto& road : roadSegments) {
        if (road) {
            auto spawnPoint = std::make_shared<SpawnPoint>(road, 10.0f, spawnRateDist(gen));
            roadNetwork.addSpawnPoint(spawnPoint);
        }
    }

    std::cout << "Network built with " << roadSegments.size() << " road segments and " << roadNetwork.getAllJunctions().size() << " junctions" << std::endl;
};


void SimulationModel::buildGridNetwork(int width, int height, float roadLength, float roadWidth, float speedLimit) {
    roadNetwork.buildNetwork(width, height, roadLength, roadWidth, speedLimit);
}