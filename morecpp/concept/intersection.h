#pragma once

#include <map>
#include "junction.h"
#include "trafficLight.h"


enum class IntersectionType {
	STOP_LIGHT,
	STOP_SIGN,
	YIELD,
	ROUNDABOUT
};


enum class TurnType {
	LEFT,
	RIGHT,
	STRAIGHT,
	U_TURN
};


struct LaneConnection {
	int fromRoadId;
	int fromLaneIdx;
	int toRoadId;
	int toLaneIdx;
	TurnType turnType;
};


struct PathOption {
	std::shared_ptr<RoadSegment> toRoad;
	int toLane;
	TurnType turnType;
	float angle;
};


class Intersection : public Junction {
private:
	IntersectionType type;
	std::vector<LaneConnection> allowedConnections;
	std::vector<std::shared_ptr<TrafficLight>> trafficLights;

	struct SignalPhase {
		float duration;
		std::vector<std::pair<int, int>> allowedRoadPairs; // fromRoadId, toRoadId
	};

	std::vector<SignalPhase> signalPhases;
	int currentPhse;
	float phseTimer;


public:
	Intersection(const std::string& id, const Vector3& pos, IntersectionType type);
	
	void addLaneConnection(int fromRoad, int fromLane, int toRoad, int toLane, TurnType turnType);
	void addTrafficLight(std::shared_ptr<TrafficLight> light);
	void defineSignalPhase(float duration, const std::vector<std::pair<int, int>>& allowedRoadPairs);

	std::vector<PathOption> getPathOptionsForLane(int roadId, int laneIdx);

	bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override;
	
	void update(float deltaTime) override;
	bool isTurnAllowed(int fromRoad, int fromLane, int toRoad, int toLane);
};