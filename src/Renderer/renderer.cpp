#include "Renderer/renderer.h"

// Constructor for the Renderer
Renderer::Renderer() 
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
      window(nullptr),
      deltaTime(0.0f) { }

void Renderer::init() {
    initGlfwWindow();
    createGlfwWindow(SCR_WIDTH, SCR_HEIGHT, APP_NAME);
    loadGLAD();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);


    ourShader.load(VSHADER_PATH, FSHADER_PATH);
}

void Renderer::drawFlatSurface() {

}

// Generate a sphere on the screen
void Renderer::drawSphere(Sphere& sphere, glm::vec3 position) {
    sphere.Position = position;
    setupSphereVertexBuffer(sphere);
    spheres.push_back(&sphere);
    if (sphere.source) lightSphere = &sphere;
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        generateCameraView();

        glm::vec3 lightPos = lightSphere ? lightSphere->Position : glm::vec3(5.0f, 5.0f, 5.0f);
        ourShader.setVec3("lightPos", lightPos);

        ourShader.setVec3("viewPos", camera.Position);

        // Sphere rendering
        for(Sphere* s : spheres) {
            if (s == lightSphere) continue;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), s->Position);
            ourShader.setBool("source", s->source);
            ourShader.setVec3("inColor", s->Color);
            ourShader.setMat4("model", model);
            glBindVertexArray(s->mesh.VAO);
            glDrawElements(GL_TRIANGLES, s->mesh.indexCount, GL_UNSIGNED_INT, 0);
        }

        if (lightSphere) {
            Sphere* s = lightSphere;

            float t = (float)glfwGetTime();
            glm::vec3 dynColor = {
                0.5f + 0.5f * sinf(t),
                0.5f + 0.5f * sinf(t + 2.094f),   // +120°
                0.5f + 0.5f * sinf(t + 4.188f)    // +240°
            };

            glm::vec3 dynPos;
            float r  = 0.3f;                
            dynPos.x = cosf(t) * r;
            dynPos.z = sinf(t) * r * cosf(t * 0.5f); 
            dynPos.y = 1.0f + 0.5f * sinf(t * 2.0f); 

            s->Position = dynPos;

            ourShader.setVec3("lightPos", s->Position);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), s->Position);
            model = glm::scale(model, glm::vec3(0.35f));
            ourShader.setBool("source", s->source);
            ourShader.setVec3("inColor", dynColor);
            ourShader.setVec3("lightColor", dynColor);
            ourShader.setMat4("model", model);
            glBindVertexArray(s->mesh.VAO);
            glDrawElements(GL_TRIANGLES, s->mesh.indexCount, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
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
}

void Renderer::setupSphereVertexBuffer(Sphere& sphere) {

    if (sphere.mesh.VAO != 0 && !sphere.remake) return;

    if (sphere.mesh.VAO == 0) {
        glGenBuffers(1, &sphere.mesh.VBO);
        glGenVertexArrays(1, &sphere.mesh.VAO);
        glGenBuffers(1, &sphere.mesh.EBO);
    }

    glBindVertexArray(sphere.mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphere.mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.geometry.getVertexDataSize(), sphere.geometry.getVertexData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.geometry.getIndexDataSize(), sphere.geometry.getIndexData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    sphere.mesh.indexCount = sphere.geometry.getIndexCount();
    sphere.remake = false;
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