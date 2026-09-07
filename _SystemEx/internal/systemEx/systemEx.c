#include "systemEx_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

iOCT_systemEx iOCT_systemEx_inst = { 0 };

// static void iOCT_input_keyCallback(OCT_index eventIndex);

void system_init_systemEx() {
    // eOCT_event_subscribe(iOCT_systemEx_inst.keyCache, OCT_LOCAL_NULL, iOCT_input_keyCallback);
}

// static void iOCT_input_keyCallback(OCT_index eventIndex) {
//     eOCT_pool buttonEventPool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_systemEx_inst.keyCache);
//     eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_systemEx_inst.keyPressCache);
//     eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_systemEx_inst.keyReleaseCache);
//
//     OCT_BUTTON button = *(int*)eOCT_field_read(buttonEventPool, iOCT_systemEx_inst.keyCache, eventIndex);
//     bool pressed = *(bool*)eOCT_field_read(buttonEventPool, iOCT_systemEx_inst.keyPressCache, eventIndex);
//     bool released = *(bool*)eOCT_field_read(buttonEventPool, iOCT_systemEx_inst.keyReleaseCache, eventIndex);
//
//     printf("Button ");
//     if (pressed) {
//         printf(" pressed\n");
//     }
//     if (released) {
//         printf(" released\n");
//         printf("AAAAAAAAAAAAAAA");
//     }
// }