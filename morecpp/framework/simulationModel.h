#pragma once

#include "../road/roadNetwork.h"


class SimulationModel {
private:
	RoadNetwork roadNetwork;
	float simulationTime = 0.0f;
	float timeScale = 1.0f;
	bool isPaused = false;

public:
	SimulationModel() = default;
	
	void update(float deltaTime);
	
	// generate different networks
	void buildCustomNetwork();
	void buildGridNetwork(int width, int height, int numLanes, float roadLength = 400.0f, float roadWidth = 32.0f, float speedLimit = 10.0f);


	// getters
	const RoadNetwork& getGridNetwork() const { return roadNetwork; }
	std::vector<std::shared_ptr<RoadSegment>> getAllRoadSegments() const { return roadNetwork.getAllRoadSegments(); }
	std::vector<std::shared_ptr<Junction>> getAllJunctions() const { return roadNetwork.getAllJunctions(); }
	bool isSimulationPaused() const { return isPaused; }
	float getTimeScale() const { return timeScale; }
	float getSimulationTime() const { return simulationTime; }


	// controls
	void pause() { isPaused = true; }
	void resume() { isPaused = false; }
	void togglePause() { isPaused = !isPaused; }
	void setTimeScale(float scale) { timeScale = scale; }

	void addRoadSegment(std::shared_ptr<RoadSegment> roadSegment) { roadNetwork.addRoadSegment(roadSegment); }
	void addJunction(std::shared_ptr<Junction> junction) { roadNetwork.addJunction(junction); }
	void addSpawnPoint(std::shared_ptr<SpawnPoint> spawnPoint) { roadNetwork.addSpawnPoint(spawnPoint); }
	void addDestination(std::shared_ptr<Destination> destination) { roadNetwork.addDestination(destination); }

};