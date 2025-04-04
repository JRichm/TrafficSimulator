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
};