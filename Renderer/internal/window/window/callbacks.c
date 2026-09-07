#include "window_int.h"
#include "window/types_int.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "../../../../Input/include/inputs.h"
#include "window/windowSystem_int.h"
#include "window/inputs/inputs_int.h"

void iOCT_window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    if (!(action == GLFW_PRESS || action == GLFW_RELEASE)) {
        return;
    }

    OCT_buttonStates octAction;
    if (action == GLFW_PRESS) {
        octAction = OCT_BUTTONSTATE_PRESSED;
    }
    else {
        octAction = OCT_BUTTONSTATE_RELEASED;
    }

    iOCT_keyEvent keyEvent = {
        .key = iOCT_getOCTButtonFromGLFW(key, false),
        .pressed = (octAction == OCT_BUTTONSTATE_PRESSED),
        .released = (octAction == OCT_BUTTONSTATE_RELEASED)
    };

    eOCT_event_broadcastGlobal(iOCT_windowSystem_inst.keyEventKey, &keyEvent);
}

void iOCT_window_mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers) {
    if (!(action == GLFW_PRESS || action == GLFW_RELEASE)) {
        return;
    }
    iOCT_mouseButtonEvent mouseButtonEvent = {
        .button = iOCT_getOCTButtonFromGLFW(button, true),
        .pressed = (action == GLFW_PRESS),
        .released = (action == GLFW_RELEASE)
    };

    eOCT_event_broadcastGlobal(iOCT_windowSystem_inst.mouseButtonEventKey, &mouseButtonEvent);
}

void iOCT_window_mouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {
    iOCT_mouseMoveEvent mouseMoveEvent = {
        .xPos = (float)xPos,
        .yPos = (float)yPos
    };

    iOCT_window* focusedWindow = eOCT_mappedPool_getByID(&iOCT_windowSystem_inst.windowMPool, iOCT_windowSystem_inst.focusedWindowID);
    focusedWindow->cursorPos = (OCT_vec2){(float)xPos, (float)yPos};
    // eOCT_event_broadcastGlobal(iOCT_windowSystem_inst.mouseMoveEventKey, &mouseMoveEvent);
}

void iOCT_window_mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    iOCT_mouseScrollEvent mouseScrollEvent = {
        .yDelta = (float)yOffset,
    };

    eOCT_event_broadcastGlobal(iOCT_windowSystem_inst.mouseScrollEventKey, &mouseScrollEvent);
}

void iOCT_window_resizeCallback(GLFWwindow* window, int width, int height) {}

void iOCT_window_focusCallback(GLFWwindow* window, int focused) {
    if (focused) {
        iOCT_windowSystem_inst.focusedWindowID = iOCT_window_findByGLFWWindowPtr(window)->windowID;
    }
}

void iOCT_window_sizeCallback(GLFWwindow* window, int width, int height) {
    iOCT_window* resized = iOCT_window_findByGLFWWindowPtr(window);
    iOCT_window_activate(*resized);

    int frameBufferX;
    int frameBufferY;
    glfwGetFramebufferSize(window, &frameBufferX, &frameBufferY);
    glViewport(0, 0, frameBufferX, frameBufferY);
    resized->currentResolution = (OCT_vec2){(float)width, (float)height};
}