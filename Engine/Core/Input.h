#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
using namespace std;


typedef struct Position {
    double x;
    double y;
} Position;


class Input {
public:
    static void Enable();
    static Position GetMousePosition();
    static Position GetMouseDelta();
    static bool IsMouseButtonPressed(int);
    static bool IsKeyPressed(int);
    static float GetDeltaTime();
    static void TimeStep();
    static void PollEvents();

private:
    static bool mouseHasMoved;
    static Position mousePosition, mouseDelta;
    static unordered_map<int, bool> mouseButtonStates, keyStates;
    static float currentTime, lastTime, deltaTime;
    static void MousePositionCallback(GLFWwindow*, double, double);
    static void MouseButtonCallback(GLFWwindow*, int, int, int);
    static void KeyCallback(GLFWwindow*, int, int, int, int);
};
