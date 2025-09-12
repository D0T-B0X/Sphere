#include "Renderer/renderer.h"

// Constructor for the Renderer
Renderer::Renderer() 
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
      window(nullptr),
      VBO(0), VAO(0), EBO(0),
      indexCount(0),
      deltaTime(0.0f) { }

void Renderer::init() {
    initGlfwWindow();
    createGlfwWindow(SCR_WIDTH, SCR_HEIGHT, APP_NAME);
    loadGLAD();

    ourShader.load(VSHADER_PATH, FSHADER_PATH);
}

void Renderer::drawElement() {

}

// Generate a sphere on the screen
void Renderer::drawSphere(CubeSphere& sphere) {
    const float* Vertices = sphere.getVertexData();
    const unsigned int* Indices = sphere.getIndexData();
    const size_t verticesSize = sphere.getVertexDataSize();
    const size_t indicesSize = sphere.getIndexDataSize();

    indexCount = sphere.getIndexCount();

    setupVertexBuffer(
        Vertices,
        Indices,
        verticesSize,
        indicesSize
    );
}

// The main render loop of the program
void Renderer::runRenderLoop() {
    while(!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        displayFrameRate(deltaTime);

        processKeyboardInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        generateCameraView();

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } 

    cleanup();
}

GLFWwindow* Renderer::getWindow() {
    return window;
}

// Initialization protocol
void Renderer::initGlfwWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Glfw window context creation
void Renderer::createGlfwWindow(unsigned int width, unsigned int height, const char* name) {
    window = glfwCreateWindow(
        width, 
        height, 
        name, 
        NULL, 
        NULL
    );

    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Renderer::loadGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
        std::exit(-1);
    }
}

void Renderer::generateCameraView() {
    glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    glm::mat4 view = camera.getViewMatrix();
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
}

void Renderer::setupVertexBuffer(const float* Vertices, const unsigned int* Indices, const size_t verticesSize, const size_t indicesSize) {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::updateVertexBuffer(CubeSphere sphere) {
    const float* vertices = sphere.getVertexData();
    const size_t verticesSize = sphere.getVertexDataSize();
    const unsigned int* indices = sphere.getIndexData();
    const size_t indicesSize = sphere.getIndexDataSize();

    indexCount = sphere.getIndexCount();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void Renderer::displayFrameRate(float deltaTime) const {

    static bool first = true;
    std::ostringstream oss;
    std::string title;

    static float timeSinceLastDisplay = 0.0f;
    timeSinceLastDisplay += deltaTime;

    if (first) {
        unsigned int frameRate = 1 / deltaTime;
        
        oss << APP_NAME << " | FPS : " << frameRate;
        title = oss.str();

        glfwSetWindowTitle(window, title.c_str());

        first = false;
    }

    if (timeSinceLastDisplay > 0.1f) {
        unsigned int frameRate = 1 / deltaTime;

        oss.clear();
        oss << APP_NAME << " | FPS : " << frameRate;
        title = oss.str();

        glfwSetWindowTitle(window, title.c_str());

        timeSinceLastDisplay = 0.0f;
    }
}

void Renderer::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (renderer) {
        renderer->handleMouse(xpos, ypos);
    }
}

void Renderer::handleMouse(double xpos, double ypos) {
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.processMouseMovement(xoffset, yoffset);
}

void Renderer::processKeyboardInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    /*
    // Control Sphere dimensions.
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        object.Sphere.setSubdivisions(object.Sphere.getSubdivisions() + 1);
        updateVertexBuffer();
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        object.Sphere.setSubdivisions(object.Sphere.getSubdivisions() - 1);
        updateVertexBuffer();
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        object.Sphere.setRadius(object.Sphere.getRadius() - 0.01f);
        updateVertexBuffer();
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        object.Sphere.setRadius(object.Sphere.getRadius() + 0.01f);
        updateVertexBuffer();
    }
    */

    // Control camera movement.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(cameraMovement::DOWN, deltaTime);
    }
}

void Renderer::cleanup() {
    ourShader.terminate();
    glfwTerminate();
}