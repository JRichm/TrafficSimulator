#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "roadSegment.h"
#include "destination.h"


class RouteManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Junction>> junctions;
	std::unordered_map<std::string, std::shared_ptr<RoadSegment>> roadSegments;


public:
	void addJunction(std::shared_ptr<Junction> junction);
	void addRoadSegment(std::shared_ptr<RoadSegment> roadSegment);

	std::vector<std::shared_ptr<RoadSegment>> findPath(std::shared_ptr<RoadSegment> currentRoad, float distanceAlongRoad, std::shared_ptr<Destination> destination);
	std::vector<std::shared_ptr<RoadSegment>> dijkstraPath(std::shared_ptr<RoadSegment> start, std::shared_ptr<RoadSegment> end);
};