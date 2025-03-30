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
#include "shader_loader.h" // Add this include


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

    // Set up rectangle vertices for rendering
    void setupRectangleVertices() {
        // Rectangle vertices (2 triangles forming a quad)
        float vertices[] = {
            // First triangle
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, 0.5f, 0.0f,  // top left
            // Second triangle
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Draw a scaled and positioned rectangle
    void drawRectangle(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color) {
        glUseProgram(shaderProgram);
        // Calculate model matrix for positioning and scaling
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        // Set uniform values
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        // View matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        // Projection matrix - orthographic for 2D
        float aspectRatio = 800.0f / 600.0f;
        glm::mat4 projection = glm::ortho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom, 0.1f, 100.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));
        // Draw the rectangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

public:
    OpenGLDisplay(int width = 800, int height = 600) : zoom(40.0f) {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }
        // Set OpenGL version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // Create window
        window = glfwCreateWindow(width, height, "Traffic Simulator - OpenGL", NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Load and compile shaders from files
        shaderProgram = createShaderProgram("default.vert", "default.frag");

        // Set up camera
        cameraPos = glm::vec3(60.0f, 6.0f, 50.0f);  // Position above the road
        cameraTarget = glm::vec3(60.0f, 6.0f, 0.0f); // Looking at center of road
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);      // Y is up
        // Set up rectangle vertices for rendering
        setupRectangleVertices();
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    ~OpenGLDisplay() {
        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);

        glfwTerminate();
    }

    bool isOpen() const {
        return !glfwWindowShouldClose(window);
    }

    bool processEvents() {
        glfwPollEvents();

        // Check for key presses for camera control
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return false;
        }

        // Camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos.x += 1.0f;
            cameraTarget.x += 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos.x -= 1.0f;
            cameraTarget.x -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos.y -= 1.0f;
            cameraTarget.y -= 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos.y += 1.0f;
            cameraTarget.y += 1.0f;
        }

        // Zoom control
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            zoom *= 1.1f;  // Zoom out
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            zoom *= 0.9f;  // Zoom in
        }

        return true;
    }

    void render(const Road& road) {
        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the road
        glm::vec3 roadPos = glm::vec3(road.getLength() / 2.0f, road.getWidth() / 2.0f, 0.0f);
        glm::vec3 roadScale = glm::vec3(road.getLength(), road.getWidth(), 1.0f);
        glm::vec3 roadColor = glm::vec3(0.3f, 0.3f, 0.3f); // Dark gray
        drawRectangle(roadPos, roadScale, roadColor);

        // Draw lane dividers
        for (int i = 1; i < road.getLaneCount(); ++i) {
            float yPos = road.getPosition().y + i * road.getLaneWidth();
            glm::vec3 dividerPos = glm::vec3(road.getLength() / 2.0f, yPos, 0.1f);
            glm::vec3 dividerScale = glm::vec3(road.getLength(), 0.2f, 1.0f);
            glm::vec3 dividerColor = glm::vec3(1.0f, 1.0f, 1.0f); // White
            drawRectangle(dividerPos, dividerScale, dividerColor);
        }

        // Draw all cars
        for (const auto& car : road.getCars()) {
            glm::vec3 carPos = glm::vec3(
                car->getPosition().x,
                car->getPosition().y,
                0.2f  // Slightly above road
            );

            glm::vec3 carScale = glm::vec3(
                car->getDimensions().x,
                car->getDimensions().y,
                1.0f
            );

            Color carColor = car->getColor();
            glm::vec3 carColorVec = glm::vec3(
                carColor.r / 255.0f,
                carColor.g / 255.0f,
                carColor.b / 255.0f
            );

            drawRectangle(carPos, carScale, carColorVec);
        }

        // Swap buffers
        glfwSwapBuffers(window);
    }
};

// Updated GameLoop class for OpenGL rendering
class OpenGLGameLoop {
public:
    static void run(Road& road, int maxIterations = 10000) {
        OpenGLDisplay display(1024, 768);

        // Timing setup
        double lastTime = glfwGetTime();
        const double targetFPS = 60.0;
        const double timeStep = 1.0 / targetFPS;

        int iteration = 0;

        while (display.isOpen() && iteration < maxIterations) {
            // Process events
            if (!display.processEvents()) {
                break;  // Window was closed
            }

            // Calculate delta time
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            if (deltaTime >= timeStep) {
                // Update game state
                road.update(static_cast<float>(timeStep));

                // Check if all cars have completed the road
                bool allCarsFinished = true;
                for (const auto& car : road.getCars()) {
                    if (car->getPosition().x < road.getLength()) {
                        allCarsFinished = false;
                        break;
                    }
                }

                if (allCarsFinished) {
                    std::cout << "All cars have completed the road." << std::endl;
                    break;
                }

                // Render current state
                display.render(road);

                lastTime = currentTime;
                iteration++;
            }
        }
    }
};