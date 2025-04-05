#pragma once

#include "openglDisplay.h"

class OpenGLGameLoop {
public:
	static void run(RoadSegment& road, int maxIterations = 10000) {
		OpenGLDisplay display;

		float lastFrame = 0.0f;
		int iteration = 0;

		while (display.isOpen() && iteration < maxIterations) {
			float currentFrame = static_cast<float>(glfwGetTime());
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (!display.processEvents()) {
				break;
			}

			road.update(deltaTime);

			display.render(road);

			iteration++;
		}
	}

    static void runNetwork(RoadNetwork& roadNetwork, int maxIterations = 10000) {
        OpenGLDisplay display;

        float lastFrame = 0.0f;
        int iteration = 0;

        while (display.isOpen() && iteration < maxIterations) {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (!display.processEvents()) {
                break;
            }

            // Update the entire road network
            roadNetwork.update(deltaTime);

            // Render the entire network instead of just one segment
            display.render(roadNetwork);

            iteration++;
        }
    }
};