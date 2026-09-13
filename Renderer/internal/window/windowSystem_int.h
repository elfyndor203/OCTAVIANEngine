#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct iOCT_windowSystem {
	OCT_ID systemID;

	eOCT_eventKey keyEventKey;
	eOCT_eventKey mouseButtonEventKey;
	// eOCT_eventKey mouseMoveEventKey;
	eOCT_eventKey mouseScrollEventKey;
	// eOCT_singleKey focusedCameraMatrixKey;
	eOCT_singleKey cursorPosKey;
	eOCT_fieldTicket transform2DTicket;

	// eOCT_IDMap windowMap;
	// eOCT_pool windowPool;
	eOCT_mappedPool windowMPool;

	GLFWwindow* rootWindow; // holds gpu resources
	OCT_ID focusedWindowID;
	// OCT_vec2 targetResolution;
	// OCT_vec2 currentResolution;
	// OCT_vec2 windowOffset;

	// OCT_key* keyMap;
	// OCT_key* mouseMap;
	// OCT_vec2 cursorPos;
	// OCT_vec2 cursorDelta;
	// OCT_vec2 scrollDelta;
};

extern iOCT_windowSystem iOCT_windowSystem_inst;
extern OCT_BUTTON iOCT_glfwToOCTButtonKeyMap[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST + 2];

eOCT_DEFINE_SINGLE_LOCAL(iOCT_cursorPos, OCT_vec2, iOCT_windowSystem_inst.cursorPosKey)

void system_init_WINDOW();
