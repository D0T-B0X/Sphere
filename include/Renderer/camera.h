#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Movement direction identifiers
enum cameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
}; 

// Default yaw angle
const float YAW = -90.0f;
// Default pitch angle
const float PITCH = 0.0f;
// Default movement speed
const float SPEED = 2.5f;
// Default mouse sensitivity
const float SENSITIVITY = 0.1f;

class Camera {
public:
    // Camera world-space position
    glm::vec3 Position;
    // Forward facing vector
    glm::vec3 Front;
    // Up direction relative to camera orientation
    glm::vec3 Up;
    // Right direction relative to camera orientation
    glm::vec3 Right;
    // World up reference vector
    glm::vec3 WorldUp;

    // Current yaw angle
    float Yaw;
    // Current pitch angle
    float Pitch;

    // Movement speed scalar
    float MovementSpeed;
    // Mouse sensitivity scalar
    float MouseSensitivity;

    // Construct from vectors and angles
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // Construct from individual float components
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Return the view matrix based on current orientation
    glm::mat4 getViewMatrix();
    // Apply keyboard movement for a direction and frame delta
    void processKeyboard(cameraMovement movement, float deltaTime);
    // Apply mouse movement offsets to yaw and pitch
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

private:
    // Recalculate orientation vectors from updated yaw and pitch
    void updateCameraVectors();
};

#endif