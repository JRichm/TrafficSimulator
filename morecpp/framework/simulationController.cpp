#include "simulationController.h"

#include <iostream>

#include <glfw/glfw3.h>


SimulationController::SimulationController() : running(false), lastFrameTime(0.0f), frameCount(0) {
	view.setSimulationModel(&model);
}


void SimulationController::init() {
	return;
}


void SimulationController::run(int maxIterations) {
	running = true;
	maxFrames = maxIterations;
	lastFrameTime = static_cast<float>(glfwGetTime());
	frameCount = 0;

	// main loop
	while (running && view.isOpen() && (maxFrames == 0 || frameCount < maxFrames)) {
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		if (!view.processEvents()) {
			running = false;
			break;
		}

		model.update(deltaTime);

		view.render();

		frameCount++;
	}
}


void SimulationController::runCustomNetworkSimulation() {
	model.buildCustomNetwork();
	run();
}


void SimulationController::runGridNetwrokSimulation(int width, int height, int numLanes) {
	model.buildGridNetwork(width, height, numLanes);
	run();
}