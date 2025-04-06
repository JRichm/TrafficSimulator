#pragma once

#include <memory>
#include <random>

#include "openglDisplay.h"
#include "../road/roadNetwork.h"
#include "../road/intersection.h"
#include "../navigation/destination.h"


class OpenGLGameLoop {
public:
	static void run(RoadSegment& road, int maxIterations = 10000) {
		OpenGLDisplay display;

		float lastFrame = 0.0f;
		int iteration = 0;

		while (display.isOpen() && iteration < maxIterations) {
			float currentFrame = static_cast<float>(glfwGetTime());
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (!display.processEvents()) {
				break;
			}

			road.update(deltaTime);

			display.render(road);

			iteration++;
		}
	}

    static void runNetwork(RoadNetwork& roadNetwork, int maxIterations = 10000) {
        OpenGLDisplay display;

        float lastFrame = 0.0f;
        int iteration = 0;

        while (display.isOpen() && iteration < maxIterations) {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (!display.processEvents()) {
                break;
            }

            roadNetwork.update(deltaTime);

            display.render(roadNetwork);

            iteration++;
        }
    }

    static void buildCustomNetwork(RoadNetwork& network) {
        network = RoadNetwork();

        // gen t-junction 
        std::cout << "Creating T-Junction..." << std::endl;
        Intersection::createTJunction(network, "t_junction_center", Vector3(0, 0, 0), 400.0f, 32.0f, 10.0f);

        // gen intersection
        std::cout << "Creating 4-way intersection..." << std::endl;
        auto fourWayJunction = Intersection::createIntersectionWithRoads(
            IntersectionType::TRAFFIC_LIGHT,
            "four_way_junction",
            Vector3(400.0f, 400.0f, 0.0f),
            { 0, 90, 180, 270 },
            300.0f,
            32.0f,
            8.0f
        );

        if (fourWayJunction) {
            network.addJunction(fourWayJunction);

            // add all roads
            for (const auto& road : fourWayJunction->getConnectedRoads()) {
                network.addRoadSegment(road);

                // add junctions
                if (auto endJunction = road->getEndJunction()) {
                    if (endJunction->getId() != fourWayJunction->getId()) {
                        network.addJunction(endJunction);
                    }
                }
            }
        }

        // gen y-junction
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
            network.addJunction(yJunction);

            // add roads
            for (const auto& road : yJunction->getConnectedRoads()) {
                network.addRoadSegment(road);

                // add junctions
                if (auto endJunction = road->getEndJunction()) {
                    if (endJunction->getId() != yJunction->getId()) {
                        network.addJunction(endJunction);
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

        network.addDestination(destination1);
        network.addDestination(destination2);
        network.addDestination(destination3);
        network.addDestination(destination4);

        // add spawn points
        std::cout << "Adding spawn points..." << std::endl;

        auto roadSegments = network.getAllRoadSegments();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> spawnRateDist(5.0f, 15.0f);

        for (const auto& road : roadSegments) {
            if (road) {
                auto spawnPoint = std::make_shared<SpawnPoint>(road, 10.0f, spawnRateDist(gen));
                network.addSpawnPoint(spawnPoint);
            }
        }

        std::cout << "Network built with " << roadSegments.size() << " road segments and "
            << network.getAllJunctions().size() << " junctions" << std::endl;
    }

    static void runCustomNetworkSimulation() {
        RoadNetwork roadNetwork;
        buildCustomNetwork(roadNetwork);
        runNetwork(roadNetwork);
    }
};