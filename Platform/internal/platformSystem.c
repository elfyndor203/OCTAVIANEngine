#include "platformSystem_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <GLFW/glfw3.h>

#include "timer/timer_int.h"

iOCT_platformSystem iOCT_platformSystem_inst = { 0 };

void iOCT_platformSystem_init() {
    glfwInit();

    *iOCT_time_get() = 0;
    glfwSetTime(0);
}

void eOCT_PLATFORM_update() {
    iOCT_timer_update();
}