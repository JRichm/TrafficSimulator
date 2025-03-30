#include "openglDisplay.h"

// Initialize static member
OpenGLDisplay* OpenGLDisplay::currentInstance = nullptr;

// Static callback for mouse scroll
void OpenGLDisplay::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (currentInstance) {
        currentInstance->processScroll(yoffset);
    }
}

// Process mouse scroll for zooming
void OpenGLDisplay::processScroll(double yoffset) {
    // Adjust zoom based on scroll direction
    float zoomSpeed = 0.1f;
    cameraPos.y += (yoffset * -1.0f) * cameraPos.y * zoomSpeed;

    // Clamp to reasonable values
    if (cameraPos.y < 5.0f) cameraPos.y = 5.0f;  // Don't get too close
    if (cameraPos.y > 150.0f) cameraPos.y = 150.0f; // Don't get too far
}

// Set up rectangle vertices for rendering
void OpenGLDisplay::setupRectangleVertices() {
    // Rectangle vertices (2 triangles forming a quad)
    float vertices[] = {
        // First triangle
        0.5f, 0.0f, 0.5f,   // top right
        0.5f, 0.0f, -0.5f,  // bottom right
        -0.5f, 0.0f, 0.5f,  // top left
        // Second triangle
        0.5f, 0.0f, -0.5f,  // bottom right
        -0.5f, 0.0f, -0.5f, // bottom left
        -0.5f, 0.0f, 0.5f   // top left
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
void OpenGLDisplay::drawRectangle(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color) {
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

    // View matrix - looking down at the scene
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // Projection matrix - orthographic for 2D-like rendering
    float aspectRatio = 800.0f / 600.0f; // Adjust to your window dimensions

    // The orthographic projection values should be based on camera height
    float orthoSize = 50.0f; // Scale factor to adjust orthographic size based on height
    // Scale it based on camera height - higher = seeing more of the scene
    float zoomFactor = cameraPos.y / 50.0f; // Normalize around a height of 50
    orthoSize *= zoomFactor;

    glm::mat4 projection = glm::ortho(
        -orthoSize * aspectRatio, orthoSize * aspectRatio,
        -orthoSize, orthoSize,
        0.1f, 200.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    // Draw the rectangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

OpenGLDisplay::OpenGLDisplay(int width, int height) : zoom(40.0f) {
    // Set current instance for callbacks
    currentInstance = this;

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

    // Register scroll callback
    glfwSetScrollCallback(window, scrollCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load and compile shaders from files
    shaderProgram = createShaderProgram("default.vert", "default.frag");

    // Set up camera
    cameraPos = glm::vec3(60.0f, 50.0f, 6.0f);  // Center of road (X), height (Y), middle of width (Z)
    cameraTarget = glm::vec3(60.0f, 0.0f, 6.0f); // Look at center of road on ground plane
    cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);      // Z is "up" in view space

    // Set up rectangle vertices for rendering
    setupRectangleVertices();
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

OpenGLDisplay::~OpenGLDisplay() {
    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
}

bool OpenGLDisplay::isOpen() const {
    return !glfwWindowShouldClose(window);
}

bool OpenGLDisplay::processEvents() {
    glfwPollEvents();

    // Check for key presses for camera control
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return false;
    }

    // Camera movement speed
    float cameraSpeed = 0.0005f;

    // Pan camera in X direction (left/right)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos.x += cameraSpeed;
        cameraTarget.x += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos.x -= cameraSpeed;
        cameraTarget.x -= cameraSpeed;
    }

    // Pan camera in Z direction (forward/backward)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos.z += cameraSpeed;
        cameraTarget.z += cameraSpeed;
    }
    // S = move down (increase Z)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos.z -= cameraSpeed;
        cameraTarget.z -= cameraSpeed;
    }

    // Zoom in/out (adjust camera height)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPos.y += cameraSpeed; // Move camera up (zoom out)
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPos.y -= cameraSpeed; // Move camera down (zoom in)
        if (cameraPos.y < 5.0f) cameraPos.y = 5.0f; // Prevent getting too close
    }

    // Always keep target directly below camera (same X,Z, but at Y=0)
    cameraTarget.x = cameraPos.x;
    cameraTarget.z = cameraPos.z;
    cameraTarget.y = 0.0f;

    return true;
}

void OpenGLDisplay::render(const Road& road) {
    // Debug output
    std::cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
    std::cout << "Camera height (zoom): " << cameraPos.y << std::endl;
    std::cout << "Road position: (" << road.getPosition().x << ", " << road.getPosition().y << ", " << road.getPosition().z << ")" << std::endl;
    std::cout << "Road dimensions: L=" << road.getLength() << ", W=" << road.getWidth() << std::endl;

    // Clear buffers
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the road - position it on the XZ plane
    glm::vec3 roadPos = glm::vec3(road.getLength() / 2.0f, 0.0f, road.getWidth() / 2.0f);
    glm::vec3 roadScale = glm::vec3(road.getLength(), 1.0f, road.getWidth());
    glm::vec3 roadColor = glm::vec3(0.3f, 0.3f, 0.3f); // Dark gray
    drawRectangle(roadPos, roadScale, roadColor);

    // Draw lane dividers - adjust Y to Z for proper positioning
    for (int i = 1; i < road.getLaneCount(); ++i) {
        float zPos = road.getPosition().z + i * road.getLaneWidth();
        glm::vec3 dividerPos = glm::vec3(road.getLength() / 2.0f, 0.1f, zPos);
        glm::vec3 dividerScale = glm::vec3(road.getLength(), 1.0f, 0.2f);
        glm::vec3 dividerColor = glm::vec3(1.0f, 1.0f, 1.0f); // White
        drawRectangle(dividerPos, dividerScale, dividerColor);
    }

    // Draw all cars - translate from XY to XZ plane
    for (const auto& car : road.getCars()) {
        glm::vec3 carPos = glm::vec3(
            car->getPosition().x,
            0.2f,  // Slightly above road
            car->getPosition().y  // Y coordinate becomes Z in 3D space
        );

        glm::vec3 carScale = glm::vec3(
            car->getDimensions().x,
            1.0f,
            car->getDimensions().y
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

// Implementation of OpenGLGameLoop
void OpenGLGameLoop::run(Road& road, int maxIterations) {
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