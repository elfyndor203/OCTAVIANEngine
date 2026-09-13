#include "systemDescription_renderer.h"

#include "OCT_Core_eng.h"

#include "renderer/renderer_int.h"
#include "renderer/sprite2D/sprite2D_int.h"
#include "renderer/camera2D/camera2D_int.h"
#include "window/window/window_int.h"
#include "window/windowSystem_int.h"
#include "window/inputs/inputs_int.h"

void system_register_RENDERER() {
    eOCT_fieldDescription screenSpace = {
        .name = "screenSpace",
        .offset = 0,
        .providerType = eOCT_DATAPATTERN_SINGLE,
        .type = eOCT_TYPE_BOOL8
    };
    eOCT_singleDescription screenSpaceSingle = {
        .name = "screenSpace",
        .providedField = screenSpace,
        .global = false,
        .keyCacheLocation = &iOCT_renderer_inst.screenSpaceKey
    };
    // eOCT_fieldDescription screenSpaceZoom = {
    //     .name = "screenSpaceZoom",
    //     .offset = 0,
    //     .providerType = eOCT_DATAPATTERN_SINGLE,
    //     .type = eOCT_DATATYPE_VEC2
    // };
    // eOCT_singleDescription screenSpaceZoomSingle = {
    //     .name = "screenSpaceZoom",
    //     .providedField = screenSpaceZoom,
    //     .global = false,
    //     .keyCacheLocation = &iOCT_renderer_inst.screenSpaceZoomKey
    // };
    eOCT_componentDescription sprite2D = {
        .name = "sprite2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = iOCT_sprite2D_root,
        .stride = sizeof(iOCT_sprite2D),
        .keyCacheLocation = &iOCT_renderer_inst.sprite2DKey,
        .sortValueOffset = offsetof(iOCT_sprite2D, sortKey),
        .entityHandleValueOffset = offsetof(iOCT_sprite2D, entityHandle)
    };
    eOCT_componentDescription camera2D = {
        .name = "camera2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,
        .keyCacheLocation = &iOCT_renderer_inst.camera2DKey,
        .sortValueOffset = eOCT_POOL_SORT_NONE,
        .stride = sizeof(iOCT_camera2D),
        .entityHandleValueOffset = offsetof(iOCT_camera2D, entityHandle)
    };

    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .type = eOCT_TYPE_MAT3,
        .ticketCache = &iOCT_renderer_inst.globalMatrix2DTicket,
        .optional = false,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };

    eOCT_systemDescription rendererSystem = {
        .name = "Renderer",
        .providedComponents = eOCT_generateComponentDescriptionPool(2, sprite2D, camera2D, eOCT_END_COMPONENTS),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_generateSingleDescriptionPool(1, screenSpaceSingle, eOCT_END_SINGLES),
        .requestedFields = eOCT_generateFieldRequestPool(1, transform2D, eOCT_END_REQUESTS),
        .systemInitFx = system_init_RENDERER,
        .contextInitFx = iOCT_renderer_contextSetup
    };

    iOCT_renderer_inst.systemID = eOCT_registry_registerSystem(rendererSystem);
}

void system_register_WINDOW() {
    eOCT_fieldDescription glfwKeys = {
        .name = "glfwKeys",
        .providerType = eOCT_DATAPATTERN_EVENT,
        .offset = offsetof(iOCT_keyEvent, key),
        .type = eOCT_TYPE_INT64
    };
    eOCT_fieldDescription glfwKeyPress = {
        .name = "glfwKeyPress",
        .providerType = eOCT_DATAPATTERN_EVENT,
        .offset = offsetof(iOCT_keyEvent, pressed),
        .type = eOCT_TYPE_BOOL8
    };
    eOCT_fieldDescription glfwKeyRelease = {
        .name = "glfwKeyRelease",
        .providerType = eOCT_DATAPATTERN_EVENT,
        .offset = offsetof(iOCT_keyEvent, released),
        .type = eOCT_TYPE_BOOL8
    };
    eOCT_fieldDescription keyFields[3] = { glfwKeys, glfwKeyPress, glfwKeyRelease };
    eOCT_eventDescription keyEvents = {
        .name = "keysEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(3, glfwKeys, glfwKeyPress, glfwKeyRelease, eOCT_END_FIELDS),
        .stride = sizeof(iOCT_keyEvent),
        .global = true,
        .keyCacheLocation = &iOCT_windowSystem_inst.keyEventKey,
    };

    eOCT_fieldDescription mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_TYPE_INT64,
        .offset = offsetof(iOCT_mouseButtonEvent, button),
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    eOCT_fieldDescription mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_TYPE_BOOL8,
        .offset = offsetof(iOCT_mouseButtonEvent, pressed),
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    eOCT_fieldDescription mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_TYPE_BOOL8,
        .offset = offsetof(iOCT_mouseButtonEvent, released),
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    eOCT_fieldDescription mouseButtonFields[3] = { mouseButton, mouseButtonPress, mouseButtonRelease };
    eOCT_eventDescription mouseButtonEvents = {
        .name = "mouseButtonEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(3, mouseButton, mouseButtonPress, mouseButtonRelease, eOCT_END_FIELDS),
        .stride = sizeof(iOCT_mouseButtonEvent),
        .global = true,
        .keyCacheLocation = &iOCT_windowSystem_inst.mouseButtonEventKey
    };

    // eOCT_fieldDescription mouseXPos = {
    //     .name = "glfwMouseXPos",
    //     .providerType = eOCT_DATAPATTERN_EVENT,
    //     .offset = offsetof(iOCT_mouseMoveEvent, xPos),
    //     .type = eOCT_DATATYPE_FLOAT32,
    // };
    // eOCT_fieldDescription mouseYPos = {
    //     .name = "glfwMouseYPos",
    //     .providerType = eOCT_DATAPATTERN_EVENT,
    //     .offset = offsetof(iOCT_mouseMoveEvent, yPos),
    //     .type = eOCT_DATATYPE_FLOAT32,
    // };
    // eOCT_eventDescription mouseMoveEvents = {
    //     .name = "mouseMoveEvents",
    //     .providedFields = eOCT_generateFieldDescriptionPool(2, mouseXPos, mouseYPos),
    //     .stride = sizeof(iOCT_mouseMoveEvent),
    //     .global = true,
    //     .keyCacheLocation = &iOCT_windowSystem_inst.mouseMoveEventKey
    // };

    eOCT_fieldDescription mouseScrollDelta = {
        .name = "glfwMouseScrollDelta",
        .providerType = eOCT_DATAPATTERN_EVENT,
        .offset = offsetof(iOCT_mouseScrollEvent, yDelta),
        .type = eOCT_TYPE_FLOAT32
    };
    eOCT_eventDescription mouseScrollEvents = {
        .name = "mouseScrollEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(1, mouseScrollDelta, eOCT_END_FIELDS),
        .stride = sizeof(iOCT_mouseScrollEvent),
        .global = true,
        .keyCacheLocation = &iOCT_windowSystem_inst.mouseScrollEventKey
    };

    // eOCT_fieldDescription screenToWorld = {
    //     .name = "screenToWorldMatrix",
    //     .providerType = eOCT_DATAPATTERN_SINGLE,
    //     .offset = 0,
    //     .type = eOCT_DATATYPE_MAT3
    // };
    // eOCT_singleDescription screenToWorldMatrix = {
    //     .name = "focusedCamera",
    //     .providedField = screenToWorld,
    //     .global = true,
    //     .keyCacheLocation = &iOCT_windowSystem_inst.focusedCameraMatrixKey
    // };
    eOCT_fieldDescription cursorPos = {
        .name = "contextCursorPos",
        .providerType = eOCT_DATAPATTERN_SINGLE,
        .offset = 0,
        .type = eOCT_TYPE_VEC2
    };
    eOCT_singleDescription cursorPosSingle = {
        .name = "contextCursorPosSingle",
        .global = false,
        .keyCacheLocation = &iOCT_windowSystem_inst.cursorPosKey,
        .providedField = cursorPos
    };
    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .optional = false,
        .ticketCache = &iOCT_windowSystem_inst.transform2DTicket,
        .type = eOCT_TYPE_MAT3,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };
    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedComponents = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_generateEventDescriptionPool(3, keyEvents, mouseButtonEvents, mouseScrollEvents, eOCT_END_EVENTS),
        .providedSingles = eOCT_generateSingleDescriptionPool(1, cursorPosSingle, eOCT_END_SINGLES),
        .requestedFields = eOCT_generateFieldRequestPool(1, transform2D, eOCT_END_REQUESTS),
        .systemInitFx = system_init_WINDOW
    };

    iOCT_windowSystem_inst.systemID = eOCT_registry_registerSystem(windowSystem);
}