#pragma once

#include "roadSegment.h"

enum class RampType {
	ENTRANCE,
	EXIT
};


struct MergeInfo {
	float mergeStartDistance;
	float mergeEndDistance;
	int targetLane;
	float mergeAngle;
};


class HighwayRamp : public RoadSegment {
private:
	RampType type;
	std::weak_ptr<RoadSegment> mainRoad;
	float mergeStartDistance;
	float mergeEndDistance;
	int targetLane;


public:
	HighwayRamp(const std::string& id, const Vector3& pos, const Vector3& dim, float speedLimit, RampType type);

	void setMainRoad(std::shared_ptr<RoadSegment> road, float startDist, float endDist, int lane);

	RampType getType() const;
	std::weak_ptr<RoadSegment> getMainRoad() const;
	MergeInfo getMergeParameters() const;
	float getMergeAngle() const;

	int getTargetLane() const;
	float getMergeStartDistance() const;
	float getMergeEndDistance() const;
};