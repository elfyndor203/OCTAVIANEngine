#include "inputs_int.h"
#include "window/types_int.h"

#include "OCT_Core_eng.h"

#include "window/windowSystem_int.h"
#include "window/window/window_int.h"
#include "renderer/renderer_int.h"

OCT_BUTTON iOCT_glfwToOCTButtonKeyMap[GLFW_KEY_LAST + GLFW_MOUSE_BUTTON_LAST + 2];

OCT_BUTTON iOCT_getOCTButtonFromGLFW(int glfwKeyCode, bool mouse) {
    return iOCT_glfwToOCTButtonKeyMap[glfwKeyCode + (mouse * iOCT_GLFW_MOUSE_BUTTONS_OFFSET)];
}

OCT_vec2 OCT_cursor_readPosContext(OCT_global context) {
    OCT_vec2* cursorPos = &eOCT_single_getLocal(iOCT_windowSystem_inst.cursorPosKey, context)->vec2;
    return *cursorPos;
}

OCT_vec2 iOCT_cursor_calcPosContext(OCT_global context) {
    if (iOCT_windowSystem_inst.focusedWindowID == OCT_ID_NULL) {
        return OCT_VEC2_NULL;
    }
    iOCT_window* focusedWindow = eOCT_mappedPool_getByID(&iOCT_windowSystem_inst.windowMPool, iOCT_windowSystem_inst.focusedWindowID);
    OCT_vec2 posWindow = focusedWindow->cursorPos;

    OCT_local contextRoot = OCT_entityContext_getRoot(context);
    OCT_mat3 rootTransform = *iOCT_globalMatrix2D_getField(contextRoot);
    OCT_mat3 invRootTransform = OCT_mat3_inv(rootTransform);
    OCT_mat3 rootRelativeMat3;
    if (eOCT_single_getLocal(iOCT_renderer_inst.screenSpaceKey, context)->boolean == false) {       // world space contexts
        OCT_mat3 screenToWorldMatrix = iOCT_window_screenToWorld(*focusedWindow);
        rootRelativeMat3 = OCT_mat3_mul(invRootTransform, screenToWorldMatrix);
    } else {                                                                                        // screen space contexts
        OCT_mat3 screenToScreenSpaceMatrix = iOCT_window_screenToScreenSpace(*focusedWindow);
        rootRelativeMat3 = OCT_mat3_mul(invRootTransform, screenToScreenSpaceMatrix);
    }

    OCT_vec3 posVec3 = OCT_mat3_mulVec3(rootRelativeMat3, (OCT_vec3){posWindow.x, posWindow.y, 1.0f});
    OCT_vec2 posVec2 = (OCT_vec2){posVec3.x, posVec3.y};
    return posVec2;
}
// Claude generated
void iOCT_buttonList_init() {
    OCT_BUTTON* array = iOCT_glfwToOCTButtonKeyMap;
    // Digits
    array[GLFW_KEY_0] = OCT_KEY_0;
    array[GLFW_KEY_1] = OCT_KEY_1;
    array[GLFW_KEY_2] = OCT_KEY_2;
    array[GLFW_KEY_3] = OCT_KEY_3;
    array[GLFW_KEY_4] = OCT_KEY_4;
    array[GLFW_KEY_5] = OCT_KEY_5;
    array[GLFW_KEY_6] = OCT_KEY_6;
    array[GLFW_KEY_7] = OCT_KEY_7;
    array[GLFW_KEY_8] = OCT_KEY_8;
    array[GLFW_KEY_9] = OCT_KEY_9;

    // Letters
    array[GLFW_KEY_A] = OCT_KEY_A;
    array[GLFW_KEY_B] = OCT_KEY_B;
    array[GLFW_KEY_C] = OCT_KEY_C;
    array[GLFW_KEY_D] = OCT_KEY_D;
    array[GLFW_KEY_E] = OCT_KEY_E;
    array[GLFW_KEY_F] = OCT_KEY_F;
    array[GLFW_KEY_G] = OCT_KEY_G;
    array[GLFW_KEY_H] = OCT_KEY_H;
    array[GLFW_KEY_I] = OCT_KEY_I;
    array[GLFW_KEY_J] = OCT_KEY_J;
    array[GLFW_KEY_K] = OCT_KEY_K;
    array[GLFW_KEY_L] = OCT_KEY_L;
    array[GLFW_KEY_M] = OCT_KEY_M;
    array[GLFW_KEY_N] = OCT_KEY_N;
    array[GLFW_KEY_O] = OCT_KEY_O;
    array[GLFW_KEY_P] = OCT_KEY_P;
    array[GLFW_KEY_Q] = OCT_KEY_Q;
    array[GLFW_KEY_R] = OCT_KEY_R;
    array[GLFW_KEY_S] = OCT_KEY_S;
    array[GLFW_KEY_T] = OCT_KEY_T;
    array[GLFW_KEY_U] = OCT_KEY_U;
    array[GLFW_KEY_V] = OCT_KEY_V;
    array[GLFW_KEY_W] = OCT_KEY_W;
    array[GLFW_KEY_X] = OCT_KEY_X;
    array[GLFW_KEY_Y] = OCT_KEY_Y;
    array[GLFW_KEY_Z] = OCT_KEY_Z;

    // Function keys
    array[GLFW_KEY_F1]  = OCT_KEY_F1;
    array[GLFW_KEY_F2]  = OCT_KEY_F2;
    array[GLFW_KEY_F3]  = OCT_KEY_F3;
    array[GLFW_KEY_F4]  = OCT_KEY_F4;
    array[GLFW_KEY_F5]  = OCT_KEY_F5;
    array[GLFW_KEY_F6]  = OCT_KEY_F6;
    array[GLFW_KEY_F7]  = OCT_KEY_F7;
    array[GLFW_KEY_F8]  = OCT_KEY_F8;
    array[GLFW_KEY_F9]  = OCT_KEY_F9;
    array[GLFW_KEY_F10] = OCT_KEY_F10;
    array[GLFW_KEY_F11] = OCT_KEY_F11;
    array[GLFW_KEY_F12] = OCT_KEY_F12;

    // Punctuation / symbols
    array[GLFW_KEY_SPACE]         = OCT_KEY_SPACE;
    array[GLFW_KEY_SEMICOLON]     = OCT_KEY_SEMICOLON;
    array[GLFW_KEY_APOSTROPHE]    = OCT_KEY_APOSTROPHE;
    array[GLFW_KEY_COMMA]         = OCT_KEY_COMMA;
    array[GLFW_KEY_PERIOD]        = OCT_KEY_PERIOD;
    array[GLFW_KEY_SLASH]         = OCT_KEY_SLASH;
    array[GLFW_KEY_BACKSLASH]     = OCT_KEY_BACKSLASH;
    array[GLFW_KEY_MINUS]         = OCT_KEY_MINUS;
    array[GLFW_KEY_EQUAL]         = OCT_KEY_EQUAL;
    array[GLFW_KEY_GRAVE_ACCENT]  = OCT_KEY_GRAVE;
    array[GLFW_KEY_LEFT_BRACKET]  = OCT_KEY_LEFT_BRACKET;
    array[GLFW_KEY_RIGHT_BRACKET] = OCT_KEY_RIGHT_BRACKET;

    // Editing / navigation
    array[GLFW_KEY_BACKSPACE]    = OCT_KEY_BACKSPACE;
    array[GLFW_KEY_DELETE]       = OCT_KEY_DELETE;
    array[GLFW_KEY_INSERT]       = OCT_KEY_INSERT;
    array[GLFW_KEY_TAB]          = OCT_KEY_TAB;
    array[GLFW_KEY_ENTER]        = OCT_KEY_ENTER;
    array[GLFW_KEY_ESCAPE]       = OCT_KEY_ESCAPE;
    array[GLFW_KEY_CAPS_LOCK]    = OCT_KEY_CAPS_LOCK;
    array[GLFW_KEY_NUM_LOCK]     = OCT_KEY_NUM_LOCK;
    array[GLFW_KEY_SCROLL_LOCK]  = OCT_KEY_SCROLL_LOCK;
    // OCT_KEY_FN_LOCK has no GLFW equivalent - left unmapped.
    array[GLFW_KEY_PRINT_SCREEN] = OCT_KEY_PRINT_SCREEN;
    array[GLFW_KEY_HOME]         = OCT_KEY_HOME;
    array[GLFW_KEY_END]          = OCT_KEY_END;
    array[GLFW_KEY_PAGE_UP]      = OCT_KEY_PAGE_UP;
    array[GLFW_KEY_PAGE_DOWN]    = OCT_KEY_PAGE_DOWN;
    array[GLFW_KEY_LEFT]         = OCT_KEY_LEFT;
    array[GLFW_KEY_RIGHT]        = OCT_KEY_RIGHT;
    array[GLFW_KEY_UP]           = OCT_KEY_UP;
    array[GLFW_KEY_DOWN]         = OCT_KEY_DOWN;

    // Modifiers - GLFW has left/right variants, OCT has one generic entry each.
    array[GLFW_KEY_LEFT_SHIFT]    = OCT_KEY_SHIFT;
    array[GLFW_KEY_RIGHT_SHIFT]   = OCT_KEY_SHIFT;
    array[GLFW_KEY_LEFT_CONTROL]  = OCT_KEY_CTRL;
    array[GLFW_KEY_RIGHT_CONTROL] = OCT_KEY_CTRL;
    array[GLFW_KEY_LEFT_ALT]      = OCT_KEY_ALT;
    array[GLFW_KEY_RIGHT_ALT]     = OCT_KEY_ALT;
    // OCT_KEY_FN has no GLFW equivalent - left unmapped.
    array[GLFW_KEY_LEFT_SUPER]    = OCT_KEY_META;
    array[GLFW_KEY_RIGHT_SUPER]   = OCT_KEY_META;
    array[GLFW_KEY_MENU]          = OCT_KEY_MENU;

    // Mouse buttons, offset into the same array.
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_LEFT]   = OCT_MOUSE_LEFT;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_RIGHT]  = OCT_MOUSE_RIGHT;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_MIDDLE] = OCT_MOUSE_MIDDLE;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_4]      = OCT_MOUSE_4;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_5]      = OCT_MOUSE_5;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_6]      = OCT_MOUSE_6;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_7]      = OCT_MOUSE_7;
    array[iOCT_GLFW_MOUSE_BUTTONS_OFFSET + GLFW_MOUSE_BUTTON_8]      = OCT_MOUSE_8;
    // GLFW only defines mouse buttons 1-8 (indices 0-7).
    // OCT_MOUSE_9 through OCT_MOUSE_12 have no GLFW mouse button to map to.
    // OCT_BUTTON_SCROLL_UP / OCT_BUTTON_SCROLL_DOWN aren't GLFW button codes at all;
    // scroll comes through glfwSetScrollCallback instead, so these stay unmapped here.
}

