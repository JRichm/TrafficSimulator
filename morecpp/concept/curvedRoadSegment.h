#pragma once

#include "roadSegment.h"


class CurvedRoadSegment : public RoadSegment {
private:
	std::vector<Vector3> controlPoints;


public:
	CurvedRoadSegment(const std::string& id, float speedLimit, const std::vector<Vector3>& controls);

	Vector3 getPositionAt(float distance) const override;
	Vector3 getDirectionAt(float distance) const override;
	Vector3 getLanePositionAt(int laneIndex, float distance) const override;

	Vector3 calculateBezierPoint(float t) const;
};