#include "events_int.h"

#include <string.h>

#include "registry/registry_int.h"
#include "layout/types.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "ECS/types_eng.h"
#include "ECS/ECS_int.h"
#include "globals/globals_int.h"

static eOCT_pool* iOCT_getEventPool(eOCT_eventDescription eventDesc, OCT_local contextHandle, eOCT_pool** callbackPoolOut);

iOCT_eventManager iOCT_eventManager_open(OCT_ID ownerID) {
    eOCT_pool eventDescPool = iOCT_registry_inst.globalEvents;
    eOCT_eventDescription* eventDescArray = (eOCT_eventDescription*)eventDescPool.array;

    eOCT_pool eventPools = eOCT_pool_open(ownerID, eventDescPool.count, sizeof(eOCT_pool));
    eOCT_pool callbackPools = eOCT_pool_open(ownerID, eventDescPool.count, sizeof(eOCT_pool));

    for (OCT_index eventCtr = 0; eventCtr < eventDescPool.count; eventCtr++) {
        eOCT_eventDescription eventDesc = eventDescArray[eventCtr];
        eOCT_pool events = eOCT_pool_open(ownerID, eOCT_POOL_CAPACITY_DEFAULT, eventDesc.stride);
        eOCT_pool callbacks = eOCT_pool_open(ownerID, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_eventCallbackFx));

        // eOCT_pool* eventSlot = (eOCT_pool*)eOCT_pool_addEntryOld(&eventPools, NULL);
        // *eventSlot = events;
        // eOCT_pool* callbackSlot = (eOCT_pool*)eOCT_pool_addEntryOld(&callbackPools, NULL);
        // *callbackSlot = callbacks;

        eOCT_pool_addEntryNew(&eventPools, &events, NULL);
        eOCT_pool_addEntryNew(&callbackPools, &callbacks, NULL);
    }

    iOCT_eventManager manager = {
        .eventPools = eventPools,
        .callbackPools = callbackPools
    };

    return manager;
}

void iOCT_eventManager_clear(iOCT_eventManager* manager) {
    for (OCT_index eventTypeCtr = 0; eventTypeCtr < manager->eventPools.count; eventTypeCtr++) {
        eOCT_pool* eventPool = (eOCT_pool*)eOCT_pool_access(&manager->eventPools, eventTypeCtr, 0);
        eOCT_pool_clear(eventPool);
    }
}

void eOCT_event_broadcastGlobal(eOCT_eventKey eventKey, void* event) {
    eOCT_pool* eventPool;
    eOCT_pool* callbackPool;
    eventPool = eOCT_event_getPoolGlobal(eventKey, &callbackPool);

    // add to the frame's events
    OCT_index eventIndex;
    void* dataLoc = eOCT_pool_addEntryOld(eventPool, &eventIndex);
    memcpy(dataLoc, event, eventPool->elementSize); // __NOTE__ maybe store stride? but it should be the same

    // call all callbacks
    eOCT_eventCallbackFx* callbackArray = (eOCT_eventCallbackFx*)callbackPool->array;
    for (OCT_index callbackCtr = 0; callbackCtr < callbackPool->count; callbackCtr++) {
        callbackArray[callbackCtr](eventIndex);
    }
}

void eOCT_event_subscribe(eOCT_fieldTicket eventField, OCT_local contextHandle, eOCT_eventCallbackFx callback) {
    if (eventField.providerType != eOCT_DATAPATTERN_EVENT) {
        OCT_ERROR_LOG(OCT_EXIT_SOURCE_MISMATCH, "Tried to set callback for field that does not come from an event");
        return;
    }
    if (!eventField.global) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
    }

    eOCT_pool* callbackPool = (eOCT_pool*)eOCT_pool_access(&iOCT_globals_inst.globalEvents.callbackPools, eventField.providerTypeIndex, 0);
    eOCT_eventCallbackFx* destination = (eOCT_eventCallbackFx*)eOCT_pool_addEntryOld(callbackPool, NULL);
    *destination = callback;
}

// void* eOCT_event_read(eOCT_fieldTicket eventField, OCT_handle contextHandle, OCT_index eventEntryIndex) {
//     if (eventField.providerType != eOCT_FIELDPROVIDER_EVENT) {
//         OCT_ERROR_LOG(OCT_EXIT_SOURCE_MISMATCH, "Tried to read ");
//         return;
//     }
//     if (!eventField.global) {
//         OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
//     }
//     return eOCT_pool_access(eventPool, eventEntryIndex, eventField.fieldOffset_reg);
// }

static eOCT_pool* iOCT_getEventPool(eOCT_eventDescription eventDesc, OCT_local contextHandle, eOCT_pool** callbackPoolOut) {
    iOCT_eventManager* eventManager;
    eOCT_pool* eventPool;
    eOCT_pool* callbackPool;
    OCT_index providerIndex = eventDesc.eventTypeIndex_reg;
    if (eventDesc.global) {
        eventManager = &iOCT_globals_inst.globalEvents;
    }
    else {
        iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
        eventManager = &context->events;
    }
    eventPool = (eOCT_pool*)eOCT_pool_access(&eventManager->eventPools, providerIndex, 0);
    callbackPool = (eOCT_pool*)eOCT_pool_access(&eventManager->callbackPools, providerIndex, 0);

    if (callbackPoolOut) {
        *callbackPoolOut = callbackPool;
    }
    return eventPool;
}