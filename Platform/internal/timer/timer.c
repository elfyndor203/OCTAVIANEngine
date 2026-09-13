#include "timer/timer.h"
#include "timer/timer_int.h"

#include "OCT_Core_eng.h"
#include <GLFW/glfw3.h>

#include "platformSystem_int.h"

// void iOCT_timer_init() {
// 	QueryPerformanceFrequency(&iOCT_PLTModule_instance.machineFreq);
// 	QueryPerformanceCounter(&iOCT_PLTModule_instance.prevFrameCt);
// 	QueryPerformanceCounter(&iOCT_PLTModule_instance.frameCt);
// }
//
// void iOCT_timer_update() {
// 	iOCT_PLTModule_instance.prevFrameCt = iOCT_PLTModule_instance.frameCt;
// 	QueryPerformanceCounter(&iOCT_PLTModule_instance.frameCt);
// }

// double OCT_timer_getDeltaTime() {
// 	return (double)(iOCT_PLTModule_instance.frameCt.QuadPart - iOCT_PLTModule_instance.prevFrameCt.QuadPart) / (double)(iOCT_PLTModule_instance.machineFreq.QuadPart);
// }

void iOCT_timer_update() {
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - iOCT_platformSystem_inst.previousFrameTime;

	*iOCT_time_get() = currentTime;
	*iOCT_deltaTime_get() = deltaTime;

	iOCT_platformSystem_inst.previousFrameTime = currentTime;
}