#include "windowSystem_int.h"

#include "OCT_Math.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdbool.h>
#include <stdlib.h>

#include "window/window_int.h"
#include "inputs/inputs_int.h"

iOCT_windowSystem iOCT_windowSystem_inst = { 0 };

void system_init_WINDOW() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* initWindow = glfwCreateWindow(100, 100, "OCTAVIAN Init Window", NULL, NULL);
	glfwMakeContextCurrent(initWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	// happens after registry init so using systemID is safe
	iOCT_windowSystem_inst.windowMPool = eOCT_mappedPool_open(iOCT_windowSystem_inst.systemID,
	                                                         eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_window),
	                                                         offsetof(iOCT_window, windowID));
	iOCT_windowSystem_inst.rootWindow = initWindow;

	iOCT_buttonList_init();
}


void eOCT_WINDOW_startFrame() {
	iOCT_window* window;
	eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowMPool.pool;
	iOCT_window* windowArray = (iOCT_window*)windowPool->array;

	for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
		window = &windowArray[windowCtr];
		glfwMakeContextCurrent(window->windowPtr);
		iOCT_window_poll(window);

		glClear(GL_COLOR_BUFFER_BIT);

		// check if windows should close
		if (glfwWindowShouldClose(window->windowPtr)) {
			iOCT_window_close(window);
			windowCtr--;
		}

	}
	if (windowPool->count < 1) {
		iOCT_windowSystem_inst.focusedWindowID = OCT_ID_NULL;
	}

	// if (iOCT_windowSystem_inst.focusedWindowID != OCT_ID_NULL) {
	// 	// iOCT_window focusedWindow = *(iOCT_window*)eOCT_getByID(&iOCT_windowSystem_inst.windowMPool.IDMap, &iOCT_windowSystem_inst.windowMPool.pool, iOCT_windowSystem_inst.focusedWindowID);
	// 	iOCT_window focusedWindow = *(iOCT_window*)eOCT_mappedPool_getByID(&iOCT_windowSystem_inst.windowMPool, iOCT_windowSystem_inst.focusedWindowID);
	// 	OCT_mat3* matrixSingle = &eOCT_single_getGlobal(iOCT_windowSystem_inst.focusedCameraMatrixKey)->mat3;
	// 	*matrixSingle = iOCT_window_screenToWorld(focusedWindow);
	// }
}

void eOCT_WINDOW_update(OCT_global context) {
	OCT_vec2* cursorPosLoc = &eOCT_single_getLocal(iOCT_windowSystem_inst.cursorPosKey, context)->vec2;

	OCT_vec2 cursorPos = iOCT_cursor_calcPosContext(context);
	*cursorPosLoc = cursorPos;
}

void eOCT_WINDOW_finishFrame() {
	eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowMPool.pool;
	iOCT_window* windowArray = (iOCT_window*)windowPool->array;

	for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
		iOCT_window window = windowArray[windowCtr];
		glfwMakeContextCurrent(window.windowPtr);

		glfwSwapBuffers(window.windowPtr);
	}
}