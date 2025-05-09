#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>


/// Manages camera position, orientation, input handling, and projection settings.
class Camera {
public:
    /// Initializes the camera at a given position.
    static void TurnOn(glm::vec3 position);

    /// Updates the camera's position and orientation from input.
    static void Update();

    /// Returns the view matrix for the current camera position and orientation.
    static glm::mat4 GetViewMatrix();

    /// Returns the perspective projection matrix based on camera FOV and window aspect ratio.
    static glm::mat4 GetProjectionMatrix();

    /// Gets the current camera position.
    static glm::vec3 GetPosition();

private:
    static glm::vec3 position;      ///< Camera position in world space.
    static glm::vec3 front;         ///< Direction the camera is facing.
    static glm::vec3 up;            ///< Camera's up vector.
    static glm::vec3 right;         ///< Camera's right vector.
    static glm::vec3 worldUp;       ///< World up vector (usually {0, 1, 0}).
    static GLfloat yawAngle;        ///< Yaw angle in degrees (horizontal rotation).
    static GLfloat pitchAngle;      ///< Pitch angle in degrees (vertical rotation).
    static GLfloat fovAngle;        ///< Field of view in degrees.
    static GLfloat movementSpeed;   ///< Movement speed for WASD motion.
    static GLfloat turnSpeed;       ///< Mouse sensitivity (degrees per pixel).

    /// Handles key presses to modify position and FOV.
    static void HandleKeyInput();

    /// Handles mouse input to modify yaw and pitch.
    static void HandleMouseInput();

    /// Recalculates the front, right, and up vectors from pitch and yaw.
    static void AdjustOrientation();
};
