#include "inputSystem_int.h"

#include "input/inputs_int.h"

iOCT_inputSystem iOCT_inputSystem_inst = {0};

static void iOCT_buttonStates_init();

void system_init_INPUT() {
    eOCT_event_subscribe(iOCT_inputSystem_inst.keyTicket, OCT_LOCAL_NULL, iOCT_input_keyCallback);
    eOCT_event_subscribe(iOCT_inputSystem_inst.mouseButtonTicket, OCT_LOCAL_NULL, iOCT_input_mouseButtonCallback);
    // eOCT_event_subscribe(iOCT_inputSystem_inst.mousePositionXTicket, OCT_LOCAL_NULL, iOCT_input_mouseMoveCallback); // __NOTE__ subscribing to two fields can result in double callbacks right now
    eOCT_event_subscribe(iOCT_inputSystem_inst.mouseScrollTicket, OCT_LOCAL_NULL, iOCT_input_mouseScrollCallback);

    iOCT_inputSystem_inst.buttonStates = eOCT_pool_open(iOCT_inputSystem_inst.systemID, (OCT_index)OCT_BUTTONS_TOTAL, sizeof(iOCT_buttonState));
    iOCT_buttonStates_init();
}

void eOCT_INPUT_update() {
    double deltaTime = *(double*)eOCT_field_read(iOCT_inputSystem_inst.deltaTimeTicket, OCT_INDEX_NULL, OCT_GLOBAL_NULL);

    for (OCT_index buttonCtr = 0; buttonCtr < OCT_BUTTONS_TOTAL; buttonCtr++) {
        iOCT_buttonState* button = eOCT_pool_access(&iOCT_inputSystem_inst.buttonStates, buttonCtr, 0);

        if (button->updatedThisFrame) {
            button->updatedThisFrame = false;
            continue;
        }

        if (button->state == OCT_BUTTONSTATE_PRESSED) {
            button->state = OCT_BUTTONSTATE_HELD;
            button->timeSinceLastStateChange = 0;
            button->framesSinceLastStateChange = 0;
        }
        if (button->state == OCT_BUTTONSTATE_RELEASED) {
            button->state = OCT_BUTTONSTATE_NONE;
            button->timeSinceLastStateChange = 0;
            button->framesSinceLastStateChange = 0;
        }
        if (button->state == OCT_BUTTONSTATE_HELD || button->state == OCT_BUTTONSTATE_NONE) {
            button->timeSinceLastStateChange += deltaTime;
            button->framesSinceLastStateChange++;
        }
    }

}

void eOCT_INPUT_clear() {
    iOCT_inputSystem_inst.mouseScrollDelta = 0;
}

static void iOCT_buttonStates_init() {
    for (OCT_index buttonCtr = 0; buttonCtr < OCT_BUTTONS_TOTAL; buttonCtr++) {
        iOCT_buttonState button = {
            .button = OCT_BUTTONS_ARRAY[buttonCtr],
            .state = OCT_BUTTONSTATE_NONE,
            .timeSinceLastStateChange = 0,
            .updatedThisFrame = false
        };
        eOCT_pool_addEntryNew(&iOCT_inputSystem_inst.buttonStates, &button, NULL);
    }
}