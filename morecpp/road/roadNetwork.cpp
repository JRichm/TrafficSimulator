#include "roadNetwork.h"

#include <random>
#include <iostream>

#include "simpleJunction.h"
#include "../traffic/car.h"


void RoadNetwork::addJunction(std::shared_ptr<Junction> junction) {
    if (junction) {
        junctions[junction->getId()] = junction;
    }
}


void RoadNetwork::addRoadSegment(std::shared_ptr<RoadSegment> roadSegment) {
    if (roadSegment) {
        roadSegments[roadSegment->getId()] = roadSegment;
    }
}


void RoadNetwork::addSpawnPoint(std::shared_ptr<SpawnPoint> spawnPoint) {
    if (spawnPoint) {
        spawnPoints.push_back(spawnPoint);
    }
}


void RoadNetwork::addDestination(std::shared_ptr<Destination> destination) {
    if (destination) {
        destinations.push_back(destination);
    }
}


std::shared_ptr<Junction> RoadNetwork::getJunction(const std::string& id) {
    auto it = junctions.find(id);
    if (it != junctions.end()) {
        return it->second;
    }
    return nullptr;
}


std::shared_ptr<RoadSegment> RoadNetwork::getRoadSegment(const std::string& id) {
    auto it = roadSegments.find(id);
    if (it != roadSegments.end()) {
        return it->second;
    }
    return nullptr;
}


void RoadNetwork::update(float deltaTime) {
    for (auto& [id, roadSegment] : roadSegments) {
        roadSegment->update(deltaTime);
    }

    for (auto& [id, junction] : junctions) {
        junction->update(deltaTime);
    }

    generateTraffic(deltaTime);
}


void RoadNetwork::generateTraffic(float deltaTime) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> spawnTimeDist(2.0f, 8.0f);
    static std::uniform_real_distribution<> speedDist(3.0f, 12.0f);
    static std::uniform_real_distribution<> colorDist(55, 255);

    for (auto& spawnPoint : spawnPoints) {
        spawnPoint->timeToNextSpawn -= deltaTime;
        
        if (spawnPoint->timeToNextSpawn <= 0) {
            spawnPoint->timeToNextSpawn = spawnTimeDist(gen);

            auto roadSegment = spawnPoint->roadSegment.lock();
            if (!roadSegment) continue;

            std::vector<int> validLanes;
            for (const auto& lane : roadSegment->getLanes()) {
                if (lane.getType() == LaneType::REGULAR) {
                    validLanes.push_back(lane.getIndex());
                }
            }

            if (validLanes.empty()) continue;

            std::uniform_int_distribution<> laneDist(0, validLanes.size() - 1);
            int laneIndex = validLanes[laneDist(gen)];

            Vector3 spawnPosition = roadSegment->getLanePositionAt(laneIndex, spawnPoint->distanceAlongRoad);

            auto car = std::make_shared<Car>(
                spawnPosition,
                Vector3(4.0f, 0.2f, 2.0f),
                Color(colorDist(gen), colorDist(gen), colorDist(gen)),
                speedDist(gen)
            );

            roadSegment->addVehicle(car);

            if (!destinations.empty()) {
                std::uniform_int_distribution<> destDist(0, destinations.size() - 1);
                car->setDestination(destinations[destDist(gen)]);
            }
        }
    }
}


std::vector<std::shared_ptr<RoadSegment>> RoadNetwork::getAllRoadSegments() const {
    std::vector<std::shared_ptr<RoadSegment>> result;
    for (const auto& [id, roadSegment] : roadSegments) {
        result.push_back(roadSegment);
    }
    return result;
}


std::vector<std::shared_ptr<Junction>> RoadNetwork::getAllJunctions() const {
    std::vector<std::shared_ptr<Junction>> result;
    for (const auto& [id, junction] : junctions) {
        result.push_back(junction);
    }
    return result;
}


bool RoadNetwork::connectRoads(const std::string& roadId1, const std::string& roadId2, const std::string& junctionId) {
    auto road1 = getRoadSegment(roadId1);
    auto road2 = getRoadSegment(roadId2);
    auto junction = getJunction(junctionId);

    if (!road1 || !road2 || junction) {
        return false;
    }

    junction->connectRoad(road1);
    junction->connectRoad(road2);

    road1->setJunctions(road1->getStartJunction(), junction);
    road2->setJunctions(junction, road2->getEndJunction());

    return true;
}


void RoadNetwork::buildNetwork(int gridWidth, int gridHeight, float roadLength, float roadWidth, float speedLimit) {
    std::cout << "Building network with dimensions: " << gridWidth << "x" << gridHeight << std::endl;
    
    // start with clean network
    junctions.clear();
    roadSegments.clear();
    spawnPoints.clear();
    destinations.clear();


    // create junction grid
    std::cout << "creating junction grid" << std::endl;
    for (int x = 0; x <= gridWidth; x++) {
        for (int y = 0; y <= gridHeight; y++) {
            std::string junctionId = "junction_" + std::to_string(x) + "_" + std::to_string(y);
            Vector3 position(x * roadLength, 0, y * roadLength);
            auto junction = std::make_shared<SimpleJunction>(junctionId, position);
            addJunction(junction);
        }
    }


    // horz roads
    std::cout << "creating horizontal roads" << std::endl;
    for (int y = 0; y <= gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            std::cout << "(" << x << ", " << y << ")" << std::endl;

            std::string roadId = "road_h_" + std::to_string(x) + "_" + std::to_string(y);

            std::string startJunctionId = "junction_" + std::to_string(x) + "_" + std::to_string(y);
            std::string endJunctionId = "junction_" + std::to_string(x + 1) + "_" + std::to_string(y);

            auto startJunction = getJunction(startJunctionId);
            auto endJunction = getJunction(endJunctionId);

            auto road = std::make_shared<RoadSegment>(roadId, startJunction->getPosition(), Vector3(roadLength, roadWidth, 0), speedLimit);

            // add lanes
            road->addLane(Lane(0, LaneType::SHOULDER, 2.0f));
            road->addLane(Lane(1, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(2, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(3, LaneType::SHOULDER, 2.0f));

            road->setJunctions(startJunction, endJunction);
            startJunction->connectRoad(road);
            endJunction->connectRoad(road);

            addRoadSegment(road);

            // add spawn points at edges
            if (x == 0 || y == 0 || x == gridWidth - 1 || y == gridHeight - 1) {
                auto spawnPoint = std::make_shared<SpawnPoint>();
                spawnPoint->roadSegment = road;
                spawnPoint->distanceAlongRoad = 0.0f;
                spawnPoint->timeToNextSpawn = 5.0f;
                addSpawnPoint(spawnPoint);
            }
        }
    }


    // vert roads
    std::cout << "creating vertical roads" << std::endl;
    for (int x = 0; x <= gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            std::string roadId = "road_v_" + std::to_string(x) + "_" + std::to_string(y);

            std::string startJunctionId = "junction_" + std::to_string(x) + "_" + std::to_string(y);
            std::string endJunctionId = "junction_" + std::to_string(x) + "_" + std::to_string(y + 1);
            auto startJunction = getJunction(startJunctionId);
            auto endJunction = getJunction(endJunctionId);

            auto road = std::make_shared<RoadSegment>(roadId, startJunction->getPosition(), Vector3(roadLength, roadWidth, 0), speedLimit);

            // add lanes
            road->addLane(Lane(0, LaneType::SHOULDER, 2.0f));
            road->addLane(Lane(1, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(2, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(3, LaneType::SHOULDER, 2.0f));

            // connect junctions
            road->setJunctions(startJunction, endJunction);
            startJunction->connectRoad(road);
            endJunction->connectRoad(road);

            addRoadSegment(road);
        }
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<>widthDist(0, gridWidth);
    static std::uniform_int_distribution<>heightDist(0, gridHeight);

    std::cout << "adding destinations" << std::endl;
    int numDestinations = std::min(5, gridWidth * gridHeight / 4);
    for (int i = 0; i < numDestinations; i++) {
        int x = widthDist(gen);
        int y = heightDist(gen);

        std::string junctionId = "junction_" + std::to_string(x) + "_" + std::to_string(y);
        auto junction = getJunction(junctionId);

        if (junction) {
            auto destination = std::make_shared<Destination>(junction->getPosition(), "Destination " + std::to_string(i));
            addDestination(destination);
        }
    }
    std::cout << "Network built with " << roadSegments.size() << " road segments and "
        << junctions.size() << " junctions" << std::endl;
}