#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "roadNetwork.h"


class OpenGLDisplay {
private:
	GLFWwindow* window;
	GLuint shaderProgram;
	GLuint VAO, VBO;

	// camera
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;
	float zoom;

	// setup
	void setupRectangleVertices();
	void setupShaders();

	// drawing
	void drawRoadSegment(const RoadSegment& road);
	void drawJunction(const Junction& junction);
	void drawVehicle(const Vehicle& vehicle);


public:
	OpenGLDisplay(int width = 800, int height = 600);
	~OpenGLDisplay();

	bool isOpen() const;
	bool processEvents();
	void render(const RoadNetwork& network);

	// camera controls
	void moveCamera(const glm::vec3& offset);
	void zoomCamera(float factor);
	void setCameraTarget(const glm::vec3& target);
};