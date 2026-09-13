#include "ECS/dataPatterns/events_eng.h"

#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"

eOCT_pool eOCT_generateEventDescriptionPool(OCT_index total, eOCT_eventDescription description1, ...) {
    if (total < 1) {
        OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no events are provided");
        return eOCT_POOL_EMPTY;
    }
    va_list args;
    va_start(args, description1);

    eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_eventDescription));
    bool end = false;
    OCT_index processed = 0;
    eOCT_eventDescription newRequest = description1;
    while (!end && processed < total) {
        if (strcmp(newRequest.name, eOCT_END_EVENTS.name) == 0) { // checks for END flag
            end = true;

            if (processed != total) {										// END flag should be after all requests are processed
                OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less events provided than expected");
                return pool;
            }
        } else {
            eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
            processed++;
            newRequest = va_arg(args, eOCT_eventDescription);
        }
    }
    eOCT_eventDescription expectedEnd = newRequest;	// most recent: either the END flag or error
    if (strcmp(expectedEnd.name, eOCT_END_EVENTS.name) != 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
        return pool;
    }

    va_end(args);
    return pool;
}

eOCT_pool* eOCT_event_getPoolGlobal(eOCT_eventKey eventKey, eOCT_pool** callbackPoolOut) {
    if (!eventKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided event is local. Accessed globally.");
        return NULL;
    }
    eOCT_pool* sourcePool = eventKey.globalEventPool;
    eOCT_pool* callbackPool = eventKey.globalCallbackPool;

    if (callbackPoolOut) {
        *callbackPoolOut = callbackPool;
    }
    return sourcePool;
}
eOCT_pool* eOCT_event_getPoolLocal(eOCT_eventKey eventKey, OCT_global contextHandle, eOCT_pool** callbackPoolOut) {
    if (eventKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided event is global. Accessed locally.");
        return NULL;
    }
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
    eOCT_pool* sourcePool = &((eOCT_pool*)context->events.eventPools.array)[eventKey.eventTypeIndex];
    eOCT_pool* callbackPool = &((eOCT_pool*)context->events.callbackPools.array)[eventKey.eventTypeIndex];

    if (callbackPoolOut) {
        *callbackPoolOut = callbackPool;
    }
    return sourcePool;
}