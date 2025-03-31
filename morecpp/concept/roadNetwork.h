#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "junction.h"
#include "roadSegment.h"
#include "spawnPoint.h"
#include "destination.h"


class RoadNetwork() {
private:
	std::unordered_map<std::string, std::shared_ptr<Junction>> junctions;
	std::unordered_map<std::string, std::shared_ptr<RoadSegment>> roadSegments;
	std::vector<std::shared_ptr<SpawnPoint>> spawnPoints;
	std::vector<std::shared_ptr<Destination>> destinations;

public:
	void addJunction(std::shared_ptr<Junction> junction);
	void addRoadSegment(std::shared_ptr<RoadSegment> roadSegment);
	void addSpawnPoint(std::shared_ptr<SpawnPoint> spawnPoint);
	void addDestination(std::shared_ptr<Destination> destination);

	std::shared_ptr<Junction> getJunction(const std::string& id);
	std::shared_ptr<RoadSegment> getRoadSegment(const std::string& id);

	void update(float deltaTime);
	void generateTraffic(float deltaTime); // don't know about this function
}