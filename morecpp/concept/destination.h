#pragma once

#include <memory>
#include <string>
#include "vec3.h"


// forward declaration
class RoadSegment;

class Destination {
private:
	std::string id;
	Vector3 position;
	std::shared_ptr<RoadSegment> connectedRoad;
	float arrivalDistance;

public:
	Destination(const std::string& id, const Vector3& pos, std::shared_ptr<RoadSegment> road, float distance);

	const std::string& getId() const;
	const Vector3& getPosition() const;
	std::shared_ptr<RoadSegment> getConnectedRoad() const;
	float getArrivalDistance() const;
};