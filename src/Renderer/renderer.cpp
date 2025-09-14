#include "Renderer/renderer.h"

// Constructor: set initial camera position and timing values
Renderer::Renderer() 
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
      window(nullptr),
      deltaTime(0.0f) { }

void Renderer::init() {
    // Init GLFW + context + GLAD, then basic GL state
    initGlfwWindow();
    createGlfwWindow(SCR_WIDTH, SCR_HEIGHT, APP_NAME);
    loadGLAD();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);               // depth testing for correct occlusion

    // Load (compile/link) main shader program
    ourShader.load(VSHADER_PATH, FSHADER_PATH);
}

// Register a sphere for rendering (lazy mesh upload / reuse)
void Renderer::drawSphere(Sphere& sphere, glm::vec3 position) {
    sphere.Position = position;
    setupSphereVertexBuffer(sphere);       // uploads only if VAO==0 or remake==true
    spheres.push_back(&sphere);
    if (sphere.source) lightSphere = &sphere; // remember light source sphere
}

// Main render loop
void Renderer::runRenderLoop() {
    while(!glfwWindowShouldClose(window)) {
        // Frame timing
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        displayFrameRate(deltaTime);
        processKeyboardInput(window);

        // Clear frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind shader + upload camera matrices
        ourShader.use();
        generateCameraView();

        // Provide light + view uniforms (light position may change below for animated light)
        glm::vec3 lightPos = lightSphere ? lightSphere->Position : glm::vec3(5.0f, 5.0f, 5.0f);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", camera.Position);

        // Draw all non-light spheres (lit objects)
        for(Sphere* s : spheres) {
            if (s == lightSphere) continue; // skip light marker here
            glm::mat4 model = glm::translate(glm::mat4(1.0f), s->Position);
            ourShader.setBool("source", s->source); // normally false here
            ourShader.setVec3("inColor", s->Color);
            ourShader.setMat4("model", model);
            glBindVertexArray(s->mesh.VAO);
            glDrawElements(GL_TRIANGLES, s->mesh.indexCount, GL_UNSIGNED_INT, 0);
        }

        // Draw / animate the light sphere (emissive)
        if (lightSphere) {
            Sphere* s = lightSphere;

            // Time parameter
            float t = (float)glfwGetTime();

            // Cycling rainbow color (phase-shifted sine)
            glm::vec3 dynColor = {
                0.5f + 0.5f * sinf(t),
                0.5f + 0.5f * sinf(t + 2.094f),   // +120°
                0.5f + 0.5f * sinf(t + 4.188f)    // +240°
            };

            // Animated "dancing" orbit path
            glm::vec3 dynPos;
            float r  = 0.3f;                
            dynPos.x = cosf(t) * r;
            dynPos.z = sinf(t) * r * cosf(t * 0.5f); 
            dynPos.y = 1.0f + 0.5f * sinf(t * 2.0f); 

            s->Position = dynPos;                // update light sphere logical position
            ourShader.setVec3("lightPos", s->Position); // refresh light position for shading

            // Build model (translate + shrink)
            glm::mat4 model = glm::translate(glm::mat4(1.0f), s->Position);
            model = glm::scale(model, glm::vec3(0.35f));

            // Source branch in fragment shader: emissive
            ourShader.setBool("source", s->source);
            ourShader.setVec3("inColor", dynColor);   // emissive tint
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

// Initialize GLFW and request core profile context
void Renderer::initGlfwWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Create window + set callbacks + enable raw mouse capture
void Renderer::createGlfwWindow(unsigned int width, unsigned int height, const char* name) {
    window = glfwCreateWindow(width, height, name, NULL, NULL);
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

// Load OpenGL function pointers via GLAD
void Renderer::loadGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
        std::exit(-1);
    }
}

// Upload projection + view matrices
void Renderer::generateCameraView() {
    glm::mat4 projection = glm::perspective(glm::radians(FOV),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    glm::mat4 view = camera.getViewMatrix();
    ourShader.setMat4("view", view);
}

// Create / update sphere mesh buffers (only when first created or remake flag true)
void Renderer::setupSphereVertexBuffer(Sphere& sphere) {

    if (sphere.mesh.VAO != 0 && !sphere.remake) return; // already uploaded and valid

    if (sphere.mesh.VAO == 0) {
        glGenBuffers(1, &sphere.mesh.VBO);
        glGenVertexArrays(1, &sphere.mesh.VAO);
        glGenBuffers(1, &sphere.mesh.EBO);
    }

    glBindVertexArray(sphere.mesh.VAO);

    // Vertex positions only (3 floats) – normals derived in shader from position
    glBindBuffer(GL_ARRAY_BUFFER, sphere.mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sphere.geometry.getVertexDataSize(),
                 sphere.geometry.getVertexData(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphere.geometry.getIndexDataSize(),
                 sphere.geometry.getIndexData(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    sphere.mesh.indexCount = sphere.geometry.getIndexCount();
    sphere.remake = false; // mesh up-to-date
}

// Update window title with FPS (throttled)
void Renderer::displayFrameRate(float deltaTime) const {
    static bool first = true;
    std::ostringstream oss;
    std::string title;
    static float timeSinceLastDisplay = 0.0f;
    timeSinceLastDisplay += deltaTime;

    if (first) {
        unsigned int frameRate = 1 / deltaTime; // (unclamped initial frame)
        oss << APP_NAME << " | FPS : " << frameRate;
        title = oss.str();
        glfwSetWindowTitle(window, title.c_str());
        first = false;
    }

    if (timeSinceLastDisplay > 0.1f) {
        unsigned int frameRate = deltaTime > 0.0f ? (unsigned int)(1.0f / deltaTime) : 0;
        oss.clear();
        oss.str("");
        oss << APP_NAME << " | FPS : " << frameRate;
        title = oss.str();
        glfwSetWindowTitle(window, title.c_str());
        timeSinceLastDisplay = 0.0f;
    }
}

// Resize callback
void Renderer::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Static mouse callback -> forward to instance
void Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->handleMouse(xpos, ypos);
    }
}

// Process raw mouse delta for camera
void Renderer::handleMouse(double xpos, double ypos) {
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    camera.processMouseMovement(xoffset, yoffset);
}

// Keyboard input mapping to camera movement
void Renderer::processKeyboardInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboard(cameraMovement::DOWN, deltaTime);
}

// Cleanup GL resources and terminate GLFW
void Renderer::cleanup() {
    ourShader.terminate();
    glfwTerminate();
}