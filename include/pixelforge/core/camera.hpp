#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>


namespace pixelforge::core {

    /// Manages camera position, orientation, input handling, and projection settings.
    class Camera {
    public:
        /// Initializes the camera at a given position.
        /// @param position the position of the camera.
        static void turnOn(glm::vec3 position);

        /// Updates the camera's position and orientation from input.
        static void update();

        /// Returns the view matrix for the current camera position and orientation.
        static glm::mat4 getViewMatrix();

        /// Returns the perspective projection matrix based on camera FOV and window aspect ratio.
        static glm::mat4 getProjectionMatrix();

        /// Gets the current camera position.
        static glm::vec3 getPosition();

    private:
        static glm::vec3 position_;      ///< Camera position in world space.
        static glm::vec3 front_;         ///< Direction the camera is facing.
        static glm::vec3 up_;            ///< Camera's up vector.
        static glm::vec3 right_;         ///< Camera's right vector.
        static glm::vec3 worldUp_;       ///< World up vector (usually {0, 1, 0}).
        static GLfloat yawAngle_;        ///< Yaw angle in degrees (horizontal rotation).
        static GLfloat pitchAngle_;      ///< Pitch angle in degrees (vertical rotation).
        static GLfloat fovAngle_;        ///< Field of view in degrees.
        static GLfloat movementSpeed_;   ///< Movement speed for WASD motion.
        static GLfloat turnSpeed_;       ///< Mouse sensitivity (degrees per pixel).

        /// Handles key presses to modify position and FOV.
        static void handleKeyInput();

        /// Handles mouse input to modify yaw and pitch.
        static void handleMouseInput();

        /// Recalculates the front, right, and up vectors from pitch and yaw.
        static void adjustOrientation();
    };

} // namespace pixelforge::core
