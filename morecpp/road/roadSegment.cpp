#include <algorithm>
#include <cmath>
#include <map>

#include "roadSegment.h"
#include "junction.h"
#include "../traffic/vehicle.h"


RoadSegment::RoadSegment(const std::string& id, const Vector3& pos, const Vector3& dim, float speedLimit)
	:	GameObject(pos, dim, Color(100, 100, 100)),
		id(id),
		length(dim.x),
		speedLimit(speedLimit) {}


void RoadSegment::setJunctions(std::shared_ptr<Junction> start, std::shared_ptr<Junction> end) {
	startJunction = start;
	endJunction = end;
}


void RoadSegment::addLane(const Lane& lane) {
	lanes.push_back(lane);
}


void RoadSegment::addLaneTransition(float startDist, float endDist, int startLanes, int endLanes, const std::map<int, int>& mapping) {
	LaneTransition transition;
	transition.startDistance = startDist;
	transition.endDistance = endDist;
	transition.startLaneCount = startLanes;
	transition.endLaneCount = endLanes;

	for (const auto& pair : mapping) {
		transition.laneMapping[pair.first] = pair.second;
	}


	laneTransitions.push_back(transition);
}


void RoadSegment::addVehicle(std::shared_ptr<Vehicle> vehicle) {
	float distanceAlongRoad = vehicle->getPosition().x;
	int lane = determineClosestLane(vehicle->getPosition().y);
	vehicle->setCurrentRoad(shared_from_this(), distanceAlongRoad, lane);
	vehicles.push_back(vehicle);
}


void RoadSegment::removeVehicle(std::shared_ptr<Vehicle> vehicle) {
	vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), vehicle), vehicles.end());
}


void RoadSegment::update(float deltaTime) {

	// loop through vehicles in this segment
	for (auto it = vehicles.begin(); it != vehicles.end();) {
		auto vehicle = *it;

		float prevDistance = vehicle->getDistanceAlongRoad();
		vehicle->update(deltaTime);

		// remove vehicle if moved to another segment
		if (vehicle->getCurrentRoad().get() != this) {
			it = vehicles.erase(it);
		}

		// if vehicle is at the end of this segment
		else if (vehicle->getDistanceAlongRoad() >= length) {

			// handle junction at end of segment
			auto junction = endJunction.lock();
			if (junction) {

				vehicle->handleIntersection(junction);

				// remove vehicle if still on segment
				if (vehicle->getCurrentRoad().get() != this) {
					it = vehicles.erase(it);
				} else {
					++it;
				}
			} else {

				// no junctions at the end, vehicles has nowhere to go
				++it;
			}
		} else {
			++it;
		}
	}
}


Vector3 RoadSegment::getLanePositionAt(int laneIndex, float distance) const {
	if (laneIndex < 0 || laneIndex >= getLaneCountAt(distance)) {
		return getPositionAt(distance);
	}

	float totalWidth = dimensions.y;
	int laneCount = getLaneCountAt(distance);
	float laneWidth = totalWidth / laneCount;

	float laneOffset = (laneIndex - (laneCount - 1) / 2.0f) * laneWidth;

	Vector3 basePos = getPositionAt(distance);

	return Vector3(basePos.x, basePos.y + laneOffset, basePos.z);
}


Vector3 RoadSegment::getDirection() const {
	auto start = startJunction.lock();
	auto end = endJunction.lock();

	if (start && end) {
		Vector3 startPos = start->getPosition();
		Vector3 endPos = end->getPosition();
		Vector3 distance = endPos - startPos;

		return distance.normalized();
	}

	bool isHorizontalRoad = dimensions.x > dimensions.y;

	return isHorizontalRoad ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(0.0f, 1.0f, 0.0f); 
}


Vector3 RoadSegment::getPerpendicular() const {
	Vector3 dir = getDirection();
	return Vector3(-dir.y, dir.x, 0.0f);
}


Vector3 RoadSegment::getDirectionVector() const {
	auto start = startJunction.lock();
	auto end = endJunction.lock();

	if (start && end) {
		Vector3 startPos = start->getPosition();
		Vector3 endPos = end->getPosition();
		Vector3 dir = endPos - startPos;
		float magnitude = dir.length();
		if (magnitude > 0.001f) {
			return Vector3(
				dir.x / magnitude,
				dir.y / magnitude,
				dir.z / magnitude
			);
		}
	}

	return Vector3(1.0f, 0, 0);
}


Vector3 RoadSegment::getPerpendicularVector() const{
	Vector3 dir = getDirectionVector();
	return Vector3(-dir.y, dir.x, 0.0f);
}


Vector3 RoadSegment::getStartPosition() const {
	if (auto start = startJunction.lock()) {
		return start->getPosition();
	}
	return position;
}


Vector3 RoadSegment::getEndPosition() const {
	if (auto end = endJunction.lock()) {
		return end->getPosition();
	}

	Vector3 dir = getDirectionVector();
	return position + dir * length;
}


Vector3 RoadSegment::getPositionAlongRoad(float distance) const {
	Vector3 startPos = getStartPosition();
	Vector3 dir = getDirectionVector();
	return startPos + dir * distance;
}


Vector3 RoadSegment::getLanePositionAlongRoad(int laneIndex, float distance) const {
	Vector3 roadPos = getPositionAlongRoad(distance);
	Vector3 perpDir = getPerpendicularVector();

	float roadWidth = dimensions.y;
	int laneCount = getLaneCountAt(distance);
	float laneWidth = roadWidth / laneCount;

	float laneOffset = (laneIndex - (laneCount - 1) / 2.0f) * laneWidth;

	return roadPos + perpDir * laneOffset;
}


float RoadSegment::getActualLength() const {
	auto start = startJunction.lock();
	auto end = endJunction.lock();

	if (start && end) {
		Vector3 startPos = start->getPosition();
		Vector3 endPos = end->getPosition();
		Vector3 distance = endPos - startPos;

		return distance.length();
	}
	return length;
}


Vector3 RoadSegment::getWorldPositionAt(int laneIndex, float distance) const {
	Vector3 roadDir = getDirection();
	Vector3 perpDir = getPerpendicular();

	Vector3 basePos = position + roadDir * distance;

	float totalWidth = dimensions.y;
	int laneCount = getLaneCountAt(distance);
	float laneWidth = totalWidth / laneCount;

	float laneOffset = (laneIndex - (laneCount - 1) / 2.0f) * laneWidth;

	return basePos + perpDir * laneOffset;
}


int RoadSegment::getLaneCountAt(float distance) const {
	for (const auto& transition : laneTransitions) {
		if (distance >= transition.startDistance && distance <= transition.endDistance) {
			float t = (distance - transition.startDistance) / (transition.endDistance - transition.startDistance);
			return std::round(transition.startLaneCount + t * (transition.endLaneCount - transition.startLaneCount));
		}
	}

	return getLaneCount();
}


int RoadSegment::getTargetLane(int currentLane, float currentDistance, float lookAheadDistance) const {
	// find distance ahead
	float targetDistance = currentDistance + lookAheadDistance;

	// limit to road length if target distance is past road length
	if (targetDistance > length) {
		targetDistance = length;
	}

	// check if target point is in a transition zone
	for (const auto& transition : laneTransitions) {
		if (currentDistance < transition.endDistance && targetDistance >= transition.startDistance) {

			// check if current lane will disappear
			if (currentLane >= transition.endLaneCount) {
				auto it = transition.laneMapping.find(currentLane);
				if (it != transition.laneMapping.end()) {
					return it->second;
				} else {
					return transition.endLaneCount - 1;
				}
			}

			// current lane still exists
			return currentLane;
		}
	}

	// stay in current lane
	return currentLane;
}


int RoadSegment::determineClosestLane(float yPosition) const {
	if (lanes.empty()) {
		return 0;
	}

	float roadCenterY = getPosition().y;

	float totalWidth = getDimensions().y;

	float topEdge = roadCenterY - totalWidth / 2.0f;

	float laneWidth = totalWidth / getLaneCount();

	float relativeY = yPosition - topEdge;
	int laneIndex = static_cast<int>(relativeY / laneWidth);

	if (laneIndex < 0) {
		laneIndex = 0;
	} else if (laneIndex >= getLaneCount()) {
		laneIndex = getLaneCount() - 1;
	}

	return laneIndex;
}


std::vector<std::shared_ptr<Vehicle>> RoadSegment::getVehiclesInLane(int laneIndex) const {
	std::vector<std::shared_ptr<Vehicle>> result;

	for (const auto& vehicle : vehicles) {
		if (vehicle->getCurrentLane() == laneIndex) {
			result.push_back(vehicle);
		}
	}

	return result;
}


std::vector<std::shared_ptr<Vehicle>> RoadSegment::getVehiclesInLaneSection(int laneIndex, float startDist, float endDist) const {
	std::vector<std::shared_ptr<Vehicle>> result;

	for (const auto& vehicle : vehicles) {
		float vehicleDist = vehicle->getDistanceAlongRoad();
		if (vehicle->getCurrentLane() == laneIndex && vehicleDist >= startDist && vehicleDist <= endDist) {
			result.push_back(vehicle);
		}
	}

	return result;
}
