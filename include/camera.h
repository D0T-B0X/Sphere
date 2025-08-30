#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum cameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
}; 

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 1.0f;

class Camera {
public:
    // camera attributes.
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler angles.
    float Yaw;
    float Pitch;

    // camera options.
    float MovementSpeed;
    float MouseSensitivity;

    // constructors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getViewMatrix();
    void processKeyboard(cameraMovement movement, float delatTime);
    void processMouseMovement(float xOffset, float yOffset, bool contrainPitch = true);

private:
    void updateCameraVectors();

};

#endif