#pragma once


#include <memory>
#include <string>

#include "../core/vec3.h"


// forward declaration
class RoadSegment;


class Destination {
private:
	std::string id;
	Vector3 position;
	std::shared_ptr<RoadSegment> connectedRoad;
	float arrivalDistance;

public:
	Destination(const std::string& id, const Vector3& pos, std::shared_ptr<RoadSegment> road, float distance)
	  :	id(id), position(pos), connectedRoad(road), arrivalDistance(distance) {}

	const std::string& getId() const { return id; }
	const Vector3& getPosition() const { return position; }
	std::shared_ptr<RoadSegment> getConnectedRoad() const { return connectedRoad; }
	float getArrivalDistance() const { return arrivalDistance; }
};