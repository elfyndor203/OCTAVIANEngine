
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"
#include "interact/interactBox2D_int.h"

void system_register_INPUT() {
    eOCT_fieldRequest key = {
        .name = "glfwKeys",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyTicket,
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    eOCT_fieldRequest keyPress = {
        .name = "glfwKeyPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyPressTicket,
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    eOCT_fieldRequest keyRelease = {
        .name = "glfwKeyRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyReleaseTicket,
        .providerType = eOCT_DATAPATTERN_EVENT,
    };
    eOCT_fieldRequest mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonTicket,
        .providerType = eOCT_DATAPATTERN_EVENT,
        .groupName_opt = "mouseButtons"
    };
    eOCT_fieldRequest mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonPressTicket,
        .providerType = eOCT_DATAPATTERN_EVENT,
        .groupName_opt = "mouseButtons"
    };
    eOCT_fieldRequest mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonReleaseTicket,
        .providerType = eOCT_DATAPATTERN_EVENT,
        .groupName_opt = "mouseButtons"
    };
    // eOCT_fieldRequest mouseMoveX = {
    //     .name = "glfwMouseXPos",
    //     .type = eOCT_DATATYPE_FLOAT32,
    //     .optional = false,
    //     .ticketCache = &iOCT_inputSystem_inst.mousePositionXTicket,
    //     .providerType = eOCT_DATAPATTERN_EVENT
    // };
    // eOCT_fieldRequest mouseMoveY = {
    //     .name = "glfwMouseYPos",
    //     .type = eOCT_DATATYPE_FLOAT32,
    //     .optional = false,
    //     .ticketCache = &iOCT_inputSystem_inst.mousePositionYTicket,
    //     .providerType = eOCT_DATAPATTERN_EVENT
    // };
    eOCT_fieldRequest mouseScroll = {
        .name = "glfwMouseScrollDelta",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseScrollTicket,
        .providerType = eOCT_DATAPATTERN_EVENT
    };
    // eOCT_fieldRequest activeCamera = {
    //     .name = "screenToWorldMatrix",
    //     .type = eOCT_DATATYPE_MAT3,
    //     .optional = false,
    //     .ticketCache = &iOCT_inputSystem_inst.cameraMatrixTicket
    // };
    eOCT_fieldRequest deltaTime = {
        .name = "deltaFrameTime",
        .type = eOCT_DATATYPE_DOUBLE64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.deltaTimeTicket,
        .providerType = eOCT_DATAPATTERN_SINGLE
    };

    eOCT_componentDescription interactBox = {
        .name = "interactBox",
        .keyCacheLocation = &iOCT_inputSystem_inst.interactBoxKey,
        .providedFields = eOCT_POOL_EMPTY,
        .entityHandleValueOffset = offsetof(iOCT_interactBox2D, entity),
        .sort = true,
        .sortValueOffset = offsetof(iOCT_interactBox2D, layer),
        .stride = sizeof(iOCT_interactBox2D)
    };

    eOCT_systemDescription inputSystem = {
        .name = "INPUT",
        .providedComponents = eOCT_generateComponentDescriptionPool(1, interactBox, eOCT_END_COMPONENTS),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(8, key, keyPress, keyRelease, mouseButton, mouseButtonPress, mouseButtonRelease, mouseScroll, deltaTime, eOCT_END_REQUESTS),
        .systemInitFx = system_init_INPUT
    };

    iOCT_inputSystem_inst.systemID = eOCT_registry_registerSystem(inputSystem);
}
