#include "Renderer/camera.h"

// Constructs a camera from position, up vector, yaw, and pitch
Camera::Camera(
    glm::vec3 position, 
    glm::vec3 up, 
    float yaw, 
    float pitch
    ) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY)
    {
        // Set initial state and compute orientation vectors
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
}

// Constructs a camera from individual float components
Camera::Camera(
    float posX,
    float posY,
    float posZ,
    float upX,
    float upY,
    float upZ,
    float yaw,
    float pitch
    ) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY)
    {
        // Set initial state and compute orientation vectors
        Position = glm::vec3(posX, posY, posZ);
        Up = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
} 

// Returns the view matrix derived from current camera transform
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

// Moves the camera based on direction and frame time
void Camera::processKeyboard(cameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == cameraMovement::FORWARD) 
        Position += Front * velocity;
    if (direction == cameraMovement::BACKWARD)
        Position -= Front * velocity;
    if (direction == cameraMovement::LEFT) 
        Position -= Right * velocity;
    if (direction == cameraMovement::RIGHT)
        Position += Right * velocity;
    if (direction == cameraMovement::UP)
        Position += Up * velocity;
    if (direction == cameraMovement::DOWN)
        Position -= Up * velocity;
}

// Adjusts yaw and pitch based on mouse movement
void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    // Clamps pitch to avoid flipping
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Recalculates direction vectors
    updateCameraVectors();
}

// Recalculates Front, Right, and Up vectors from yaw and pitch
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}