#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../road/roadSegment.h"
#include "../road/roadNetwork.h"


class OpenGLDisplay {
private:
	GLFWwindow* window;
	GLuint shaderProgram;
	GLuint VAO, VBO;

	// camera settings
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;
	float orthographicLeft = -120.0f;
	float orthographicRight = 120.0f;
	float orthographicBottom = -60.0f;
	float orthographicTop = 60.0f;

	// view settings
	float zoom;
	static OpenGLDisplay* currentInstance;

	// setup
	void setupRectangleVertices();
	void drawRectangle(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color);
	void renderRoadSegment(const RoadSegment& road);

	static void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
	void processScroll(double yoffset);


public:
	OpenGLDisplay(int width = 1920, int height = 1080);
	~OpenGLDisplay();

	bool isOpen() const;
	bool processEvents();
	void render(const RoadSegment& road);
	void render(const RoadNetwork& network);
};