#include "junction.h"

#include "roadSegment.h"


std::vector<std::shared_ptr<RoadSegment>>Junction::getConnectedRoads() const {
	std::vector<std::shared_ptr<RoadSegment>> roads;
	for (const auto& weakRoad : connectedRoads) {
		if (auto road = weakRoad.lock()) {
			roads.push_back(road);
		}
	}
	return roads;
}


int Junction::getRoadIndex(std::shared_ptr<RoadSegment> road) const {
	for (size_t i = 0; i < connectedRoads.size(); i++) {
		if (auto r = connectedRoads[i].lock()) {
			if (r->getId() == road->getId()) {
				return static_cast<int>(i);
			}
		}
	}
	return -1;
}


std::vector<std::shared_ptr<RoadSegment>> Junction::getExitRoads(std::shared_ptr<RoadSegment> entryRoad) const {
	std::vector<std::shared_ptr<RoadSegment>> exits;
	for (const auto& weakRoad : connectedRoads) {
		if (auto road = weakRoad.lock()) {
			if (road != entryRoad) {
				exits.push_back(road);
			}
		}
	}
	return exits;
}