#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "road.h"
#include "car.h"
#include "shader_loader.h"

class OpenGLDisplay {
private:
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO;
    // Camera properties
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;
    // View settings
    float zoom;
    static OpenGLDisplay* currentInstance;

    // Static callback for mouse scroll
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Process mouse scroll for zooming
    void processScroll(double yoffset);

    // Set up rectangle vertices for rendering
    void setupRectangleVertices();

    // Draw a scaled and positioned rectangle
    void drawRectangle(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color);

public:
    OpenGLDisplay(int width = 800, int height = 600);
    ~OpenGLDisplay();

    bool isOpen() const;
    bool processEvents();
    void render(const Road& road);
};

// OpenGL Game Loop class
class OpenGLGameLoop {
public:
    static void run(Road& road, int maxIterations = 10000);
};