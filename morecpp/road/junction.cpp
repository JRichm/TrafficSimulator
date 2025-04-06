#include "junction.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "roadSegment.h"




void Junction::connectRoad(std::shared_ptr<RoadSegment> road) {
	connectedRoads.push_back(road);

	Vector3 roadDir = road->getDirectionVector();
	float angle = atan2(roadDir.y, roadDir.x) * 180.0f / M_PI;
	roadAngles[road->getId()] = angle;
}


std::vector<std::shared_ptr<RoadSegment>> Junction::getConnectedRoads() const {
	std::vector<std::shared_ptr<RoadSegment>> roads;
	for (const auto& weakRoad : connectedRoads) {
		if (auto road = weakRoad.lock()) {
			roads.push_back(road);
		}
	}
	return roads;
}


float Junction::getAngleBetweenRoads(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad) const {
	if (roadAngles.find(fromRoad->getId()) == roadAngles.end() || roadAngles.find(toRoad->getId()) == roadAngles.end()) {
		return 0.0f;
	}

	float fromAngle = roadAngles.at(fromRoad->getId());
	float toAngle = roadAngles.at(toRoad->getId());

	float angleDiff = fabs(toAngle - fromAngle);
	if (angleDiff > 180.0f) {
		angleDiff = 360.0f - angleDiff;
	}

	return angleDiff;
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