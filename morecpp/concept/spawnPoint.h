#pragma once

#include <vector>
#include <memory>
#include <map>
#include "vec3.h"
#include "vehicle.h"


// forward declarations
class RoadSegment;
class Destination;

class SpawnPoint {
private: Vector3 position;
	std::shared_ptr<RoadSegment> connectedRoad;
	int startLane;
	float spawnRate;
	float spawnTimer;

	std::vector<VehicleType> allowedVehicleTypes;
	std::map<std::shared_ptr<Destination>, float> destinationWeights;

public:
	SpawnPoint(const Vector3& pos, std::shared_ptr<RoadSegment> road, int lane, float rate);

	void addAllowedVehicleType(VehicleType type);
	void addDestination(std::shared_ptr<Destination> dest, float weight);

	std::shared_ptr<Vehicle> generateVehicle();
	void update(float deltaTime);

	const Vector3& getPosition() const;
	std::shared_ptr<RoadSegment> getConnectedRoad() const;
	int getStartLane() const;
	float getSpawnRate() const;
};