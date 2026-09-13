#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

#define eOCT_END_EVENTS ((eOCT_eventDescription){.name = "EVENTS"})

struct eOCT_eventDescription { // for cross module communication, but what about for the user __NOTE__
    const char* name;
    size_t stride;
    eOCT_pool providedFields;
    eOCT_eventKey* keyCacheLocation;
    bool global;

    OCT_index eventTypeIndex_reg;
};
struct eOCT_eventKey {
    const char* name;

    OCT_index eventTypeIndex;
    bool global;
    eOCT_pool* globalEventPool;
    eOCT_pool* globalCallbackPool;
};

eOCT_pool eOCT_generateEventDescriptionPool(OCT_index total, eOCT_eventDescription description1, ...);
eOCT_pool* eOCT_event_getPoolGlobal(eOCT_eventKey eventKey, eOCT_pool** callbackPoolOut);
eOCT_pool* eOCT_event_getPoolLocal(eOCT_eventKey eventKey, OCT_global contextHandle, eOCT_pool** callbackPoolOut);
