#pragma once
#include "window/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/types_int.h"
#include "renderer/camera2D/camera2D_int.h"

struct iOCT_window {
    OCT_ID windowID;

    GLFWwindow* windowPtr;
    OCT_vec2 targetResolution;
    OCT_vec2 currentResolution;

    OCT_vec2 cursorPos;

    OCT_vec2 cursorDelta;
    OCT_local activeCameraSourceEntity;
    GLint cameraUniformLocation;

    OCT_vec2 screenSpaceZoom;

    GLuint VAO;
};

struct iOCT_keyEvent {
    int key;
    bool pressed;
    bool released;
};
struct iOCT_mouseButtonEvent {
    int button;
    bool pressed;
    bool released;
};
struct iOCT_mouseMoveEvent {
    float xPos;
    float yPos;
};
struct iOCT_mouseScrollEvent {
    float yDelta;
};

// void iOCT_window_wipe();
// void iOCT_window_show();
// void iOCT_window_viewport(int width, int height);
void iOCT_window_activate(iOCT_window window);
OCT_mat3 iOCT_window_screenToWorld(iOCT_window window);
OCT_mat3 iOCT_window_screenToScreenSpace(iOCT_window window);
OCT_mat3 iOCT_window_worldToNDC(iOCT_window window);
// OCT_mat3 iOCT_window_getCameraOnlyProj(iOCT_window window);
iOCT_window* iOCT_window_findByGLFWWindowPtr(GLFWwindow* windowPtr);

void iOCT_window_poll(iOCT_window* window);
void iOCT_window_close(iOCT_window* window);

// void iOCT_window_callback_resize(GLFWwindow* window, int newWidth, int newHeight);
void iOCT_window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void iOCT_window_mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
void iOCT_window_mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
void iOCT_window_mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void iOCT_window_focusCallback(GLFWwindow* window, int focused);
void iOCT_window_sizeCallback(GLFWwindow* window, int width, int height);

