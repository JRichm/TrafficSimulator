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
	HighwayRamp(const std::string& id, const Vector3& pos, const Vector3& dim, float speedLimit, RampType type)
	  :	RoadSegment(id, pos, dim, speedLimit),
		type(type),
		mergeStartDistance(0),
		mergeEndDistance(0),
		targetLane(0) {
	}

	void setMainRoad(std::shared_ptr<RoadSegment> road, float startDist, float endDist, int lane) {
		mainRoad = road;
		mergeStartDistance = startDist;
		mergeEndDistance = endDist;
		targetLane = lane;
	}

	RampType getType() const { return type; }
	std::weak_ptr<RoadSegment> getMainRoad() const { return mainRoad; }

	MergeInfo getMergeParameters() const {
		MergeInfo info;
		info.mergeStartDistance = mergeStartDistance;
		info.mergeEndDistance = mergeEndDistance;
		info.targetLane = targetLane;
		info.mergeAngle = 15.0f;
		return info;
	}

	float getMergeAngle() const { return 15.0f; }
	int getTargetLane() const { return targetLane; }
	float getMergeStartDistance() const { return mergeStartDistance; }
	float getMergeEndDistance() const { return mergeEndDistance; }
};