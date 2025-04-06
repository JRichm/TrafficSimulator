#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

#include "roadNetwork.h"
#include "trafficLightJunction.h"
#include "simpleJunction.h"

enum class IntersectionType {
	SIMPLE,
	TRAFFIC_LIGHT,
	ROUNDABOUT, 
	T_JUNCTION,
	Y_JUNCTION,
};


class Intersection {
public:
	static std::shared_ptr<Junction> createIntersection(IntersectionType type, const std::string& id, const Vector3& position, float radius = 15.0f) {
		switch (type) {
		case IntersectionType::TRAFFIC_LIGHT:
			return std::make_shared<TrafficLightJunction>(id, position, radius);
		case IntersectionType::SIMPLE:
			return std::make_shared<SimpleJunction>(id, position, radius);
		default:
			return std::make_shared<SimpleJunction>(id, position, radius);
		}
	}


    static std::shared_ptr<Junction> createIntersectionWithRoads(
        IntersectionType type,
        const std::string& id,
        const Vector3& position,
        const std::vector<float>& roadAngles,
        float roadLength = 400.0f,
        float roadWidth = 32.0f,
        float speedLimit = 10.0f
    ) {
        auto junction = createIntersection(type, id, position);

        if (!junction) {
            std::cerr << "Failed to create junction: " << id << std::endl;
            return nullptr;
        }

        // create angled roads
        for (size_t i = 0; i < roadAngles.size(); i++) {
            float angleRad = roadAngles[i] * M_PI / 180.0f;
            Vector3 direction(cos(angleRad), sin(angleRad), 0.0f);

            // get road start position
            Vector3 roadPos = position + direction * junction->getRadius();

            // create road
            std::string roadId = id + "_road_" + std::to_string(i);
            auto road = std::make_shared<RoadSegment>(
                roadId,
                roadPos,
                Vector3(roadLength, roadWidth, 0),
                speedLimit
            );

            // add lanes
            road->addLane(Lane(0, LaneType::SHOULDER, 2.0f));
            road->addLane(Lane(1, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(2, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(3, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(4, LaneType::SHOULDER, 2.0f));

            // create end junction
            Vector3 endPos = roadPos + direction * roadLength;
            auto endJunction = createIntersection(IntersectionType::SIMPLE,
                id + "_end_" + std::to_string(i), endPos);

            if (!endJunction) {
                std::cerr << "Failed to create end junction for road " << roadId << std::endl;
                continue;
            }

            // add connections
            road->setJunctions(junction, endJunction);
            junction->connectRoad(road);
            endJunction->connectRoad(road);
        }

        // generate phases
        if (type == IntersectionType::TRAFFIC_LIGHT) {
            auto trafficJunction = std::dynamic_pointer_cast<TrafficLightJunction>(junction);
            if (trafficJunction) {
                trafficJunction->generatePhases();
            }
        }

        return junction;
    }


	static void createTJunction(RoadNetwork& network, const std::string& id, const Vector3& position, float roadLength = 400.0f, float roadWidth = 32.0f, float speedLimit = 10.0f) {
        auto junction = createIntersection(IntersectionType::TRAFFIC_LIGHT, id, position);
        if (!junction) {
            std::cerr << "Failed to create T-junction: " << id << std::endl;
            return;
        }

        network.addJunction(junction);

        auto roadEast = std::make_shared<RoadSegment>(id + "_east", Vector3(position.x + junction->getRadius(), position.y, 0), Vector3(roadLength, roadWidth, 0), speedLimit);
        auto roadWest = std::make_shared<RoadSegment>(id + "_west", Vector3(position.x - roadLength - junction->getRadius(), position.y, 0), Vector3(roadLength, roadWidth, 0), speedLimit);
        auto roadNorth = std::make_shared<RoadSegment>(id + "_north", Vector3(position.x, position.y + junction->getRadius(), 0), Vector3(roadLength, roadWidth, 0), speedLimit);

        auto addLanes = [](std::shared_ptr<RoadSegment> road, bool isVertical) {
            if (!road) return;

            road->addLane(Lane(0, LaneType::SHOULDER, 2.0f));
            road->addLane(Lane(1, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(2, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(3, LaneType::REGULAR, 4.0f));
            road->addLane(Lane(4, LaneType::SHOULDER, 2.0f));
            };

        if (roadEast && roadWest && roadNorth) {
            addLanes(roadEast, false);
            addLanes(roadWest, false);
            addLanes(roadNorth, true);
        } else {
            std::cerr << "Failed to create one or more roads for T-junction" << std::endl;
            return;
        }

        // create end junctions
        auto eastEnd = createIntersection(IntersectionType::SIMPLE, id + "_east_end",
            Vector3(position.x + roadLength + junction->getRadius() * 2, position.y, 0));

        auto westEnd = createIntersection(IntersectionType::SIMPLE, id + "_west_end",
            Vector3(position.x - roadLength - junction->getRadius() * 2, position.y, 0));

        auto northEnd = createIntersection(IntersectionType::SIMPLE, id + "_north_end",
            Vector3(position.x, position.y + roadLength + junction->getRadius() * 2, 0));

        if (!eastEnd || !westEnd || !northEnd) {
            std::cerr << "Failed to create one or more end junctions for T-junction" << std::endl;
            return;
        }

        // add junctions
        roadEast->setJunctions(junction, eastEnd);
        roadWest->setJunctions(westEnd, junction);
        roadNorth->setJunctions(junction, northEnd);

        // connect roads
        junction->connectRoad(roadEast);
        junction->connectRoad(roadWest);
        junction->connectRoad(roadNorth);

        eastEnd->connectRoad(roadEast);
        westEnd->connectRoad(roadWest);
        northEnd->connectRoad(roadNorth);

        // add roads and junctions
        network.addRoadSegment(roadEast);
        network.addRoadSegment(roadWest);
        network.addRoadSegment(roadNorth);

        network.addJunction(eastEnd);
        network.addJunction(westEnd);
        network.addJunction(northEnd);

        // add spawn points
        auto spawnEast = std::make_shared<SpawnPoint>(roadEast, 0.0f, 8.0f);
        auto spawnWest = std::make_shared<SpawnPoint>(roadWest, 0.0f, 8.0f);
        auto spawnNorth = std::make_shared<SpawnPoint>(roadNorth, 0.0f, 8.0f);

        network.addSpawnPoint(spawnEast);
        network.addSpawnPoint(spawnWest);
        network.addSpawnPoint(spawnNorth);

        // add destinations
        auto destinationEast = std::make_shared<Destination>(eastEnd->getPosition(), "East Destination");
        auto destinationWest = std::make_shared<Destination>(westEnd->getPosition(), "West Destination");
        auto destinationNorth = std::make_shared<Destination>(northEnd->getPosition(), "North Destination");

        network.addDestination(destinationEast);
        network.addDestination(destinationWest);
        network.addDestination(destinationNorth);

        auto trafficJunction = std::dynamic_pointer_cast<TrafficLightJunction>(junction);
        if (trafficJunction) {
            trafficJunction->generatePhases();
        }
	}
};