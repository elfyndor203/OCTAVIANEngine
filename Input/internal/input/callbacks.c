
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"
#include "inputs_int.h"

static void iOCT_input_printButton(OCT_BUTTON buttons);

void iOCT_input_keyCallback(OCT_index eventIndex) {
    OCT_BUTTON button = *(int*)eOCT_field_read(iOCT_inputSystem_inst.keyTicket, eventIndex, OCT_GLOBAL_NULL);
    bool pressed = *(bool*)eOCT_field_read(iOCT_inputSystem_inst.keyPressTicket, eventIndex, OCT_GLOBAL_NULL);
    bool released = *(bool*)eOCT_field_read(iOCT_inputSystem_inst.keyReleaseTicket, eventIndex, OCT_GLOBAL_NULL);

    printf("Button ");
    iOCT_input_printButton(button);
    if (pressed) {
        printf(" pressed\n");
        iOCT_button_updateState(button, OCT_BUTTONSTATE_PRESSED);
    }
    if (released) {
        printf(" released\n");
        iOCT_button_updateState(button, OCT_BUTTONSTATE_RELEASED);
    }

}

void iOCT_input_mouseButtonCallback(OCT_index eventIndex) {
    OCT_BUTTON button = *(int*)eOCT_field_read(iOCT_inputSystem_inst.mouseButtonTicket, eventIndex, OCT_GLOBAL_NULL);
    bool pressed = *(bool*)eOCT_field_read(iOCT_inputSystem_inst.mouseButtonPressTicket, eventIndex, OCT_GLOBAL_NULL);
    bool released = *(bool*)eOCT_field_read(iOCT_inputSystem_inst.mouseButtonReleaseTicket, eventIndex, OCT_GLOBAL_NULL);

    printf("Button ");
    iOCT_input_printButton(button);
    if (pressed) {
        printf(" pressed\n");
        iOCT_button_updateState(button, OCT_BUTTONSTATE_PRESSED);
    }
    if (released) {
        printf(" released\n");
        iOCT_button_updateState(button, OCT_BUTTONSTATE_RELEASED);
    }
}

// void iOCT_input_mouseMoveCallback(OCT_index eventIndex) {
//     eOCT_pool mouseXEventPool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_inputSystem_inst.mousePositionXTicket);
//     eOCT_pool mouseYEventPool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_inputSystem_inst.mousePositionYTicket);
//
//     float xPos = *(float*)eOCT_field_read(mouseXEventPool, iOCT_inputSystem_inst.mousePositionXTicket, eventIndex);
//     float yPos = *(float*)eOCT_field_read(mouseYEventPool, iOCT_inputSystem_inst.mousePositionYTicket, eventIndex);
//
//     iOCT_inputSystem_inst.mousePosition = (OCT_vec2){xPos, yPos};
// }

void iOCT_input_mouseScrollCallback(OCT_index eventIndex) {
    float yDelta = *(float*)eOCT_field_read(iOCT_inputSystem_inst.mouseScrollTicket, eventIndex, OCT_GLOBAL_NULL);

    iOCT_inputSystem_inst.mouseScrollDelta += yDelta;

    if (yDelta > 0.0f) {
        iOCT_button_updateState(OCT_MOUSE_SCROLL_DOWN, OCT_BUTTONSTATE_PRESSED);
    }
    if (yDelta < 0.0f) {
        iOCT_button_updateState(OCT_MOUSE_SCROLL_UP, OCT_BUTTONSTATE_PRESSED);
    }
}

static void iOCT_input_printButton(OCT_BUTTON buttons) {
    const char* string = OCT_BUTTONS_NAMES[buttons];

    printf("%s", string);
}