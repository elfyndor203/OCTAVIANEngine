
#include "OCT_Core_eng.h"
#include <string.h>

#include "ECS/entityContext_int.h"

eOCT_pool* eOCT_field_getSourcePool(OCT_global contextHandle, eOCT_fieldTicket fieldTicket) {
    if (fieldTicket.global && fieldTicket.globalPool) {
        return fieldTicket.globalPool;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_COMPONENT) {
        iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
        eOCT_pool* componentPool = &((eOCT_pool*)context->components.array)[fieldTicket.providerTypeIndex];
        return componentPool;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_DATAPOOL) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Data pools are being updated\n");
        return NULL;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_EVENT) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
        return NULL;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_SINGLE) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context singles are not yet implemented\n");
        return NULL;
    }

    return NULL;
}

void* eOCT_field_read(eOCT_fieldTicket fieldTicket, OCT_index entryIndex, OCT_global contextHandle) {
    eOCT_pool* sourcePool;
    OCT_index actualIndex;
    if (fieldTicket.global && fieldTicket.globalPool) {
        sourcePool = fieldTicket.globalPool;
    }
    else {
        sourcePool = eOCT_field_getSourcePool(contextHandle, fieldTicket);
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_SINGLE) {
        actualIndex = fieldTicket.providerTypeIndex;
    }
    else if (entryIndex == OCT_INDEX_NULL) {
        OCT_ERROR_LOG(OCT_EXIT_SOURCE_MISMATCH, "Provided no entry index for a field from a non-SINGLE source");
        return NULL;
    }
    else {
        actualIndex = entryIndex;
    }
        void* fieldLoc = eOCT_pool_access(sourcePool, actualIndex, fieldTicket.offsetFromStruct);
    return fieldLoc;
}
// static void* iOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex) {
//     void* fieldLoc = eOCT_pool_access(&sourcePool, entryIndex, fieldDetails.offsetFromStruct);
//     return fieldLoc;
// }

eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_pool* sourcePool = &((eOCT_pool*)context->components.array)[componentKey.componentTypeIndex];
    return sourcePool;
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

eOCT_dataUnion* eOCT_single_getGlobal(eOCT_singleKey singleKey) {
    if (!singleKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided single is local. Accessed globally.");
        return NULL;
    }
    return (eOCT_dataUnion*)eOCT_pool_access(singleKey.globalPool, singleKey.singleTypeIndex, 0);
}

eOCT_dataUnion* eOCT_single_getLocal(eOCT_singleKey singleKey, OCT_global contextHandle) {
    if (singleKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided single is global. Accessed locally.");
        return NULL;
    }
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_dataUnion* dataLoc = eOCT_pool_access(&context->singles, singleKey.singleTypeIndex, 0);
    return dataLoc;
}

eOCT_mappedPool* eOCT_dataPool_getGlobal(eOCT_dataPoolKey dataPoolKey) {
    if (!dataPoolKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided dataPool is local. Accessed globally.");
    }
    return dataPoolKey.globalMappedPool;
}
eOCT_mappedPool* eOCT_dataPool_getLocal(eOCT_dataPoolKey dataPoolKey, OCT_global contextHandle) {
    if (dataPoolKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided dataPool is global. Accessed locally.");
    }
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
    eOCT_mappedPool* mPool = eOCT_pool_access(&context->dataPools, dataPoolKey.dataPoolTypeIndex, 0);
    return mPool;
}

// eOCT_dataUnion* eOCT_single_upload(eOCT_singleKey singleKey, void* source, OCT_handle contextHandle) {
//     eOCT_pool* sourcePool;
//     if (singleKey.global && singleKey.globalPool) {
//         sourcePool = singleKey.globalPool;
//     }
//     else {
//         sourcePool = &iOCT_entityContext_get(contextHandle.objectID)->singles;
//     }
//
//     eOCT_dataUnion* dataLoc = eOCT_pool_access(sourcePool, singleKey.singleTypeIndex, 0);
//
//     if (source) {
//         memcpy(dataLoc, source, sizeof())
//     }
// }