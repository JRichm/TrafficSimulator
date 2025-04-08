#pragma once

#include <memory>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "simulationModel.h"
#include "../road/trafficLightJunction.h"

class ViewController {
private:
	GLFWwindow* window;
	GLuint shaderProgram;
	GLuint VAO, VBO;

	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;
	float orthographicLeft = -120.0f;
	float orthographicRight = 120.0f;
	float orthographicBottom = -60.0f;
	float orthographicTop = 60.0f;
	float zoom = 40.0f;

	SimulationModel* simulationModel;

	static ViewController* currentInstance;

	void setupRectangleVerticies();
	void drawRectangle(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color);

	void renderRoadSegment(const RoadSegment& road);
	void renderJunction(const Junction& junction);
	void renderTrafficLights(const TrafficLightJunction& junction);
	void renderVehicle(const Vehicle& vehicle, const RoadSegment& road);

	static void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);
	void processScroll(double yOffset);


public:
	ViewController(int width = 1920, int height = 1080);
	~ViewController();

	void setSimulationModel(SimulationModel* model) { simulationModel = model; }
	void render();
	bool processEvents();
	bool isOpen() const { return window && !glfwWindowShouldClose(window); }

	void moveCamera(float deltaX, float deltaY);
	void zoomCamera(float zoomFactor) { processScroll(zoomFactor); }
};

