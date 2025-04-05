#pragma once


class SpawnPoint {
public:
	std::weak_ptr<RoadSegment> roadSegment;
	float distanceAlongRoad;
	float timeToNextSpawn;
	float spawnRate;

	SpawnPoint() : distanceAlongRoad(0.0f), timeToNextSpawn(0.0f), spawnRate(10.0f) {}
	SpawnPoint(std::shared_ptr<RoadSegment> road, float distance, float rate = 10.0f) :
		roadSegment(road),
		distanceAlongRoad(distance),
		timeToNextSpawn(0.0f),
		spawnRate(rate) {}
};