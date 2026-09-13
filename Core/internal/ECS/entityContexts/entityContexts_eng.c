#include "entityContexts_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "registry/registry_int.h"
#include "events/events_int.h"

eOCT_contextToken eOCT_context_getToken(OCT_global contextHandle) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_contextToken newToken = {
        .contextPtr = context,
        .entityMap = &context->entityIDMap,
        .entities = &context->entities,
        .components = &context->components,
        .valid = true
    };

    return newToken;
}
void eOCT_context_invalidateToken(eOCT_contextToken* token) {
    token->contextPtr = NULL;
    token->valid = false;
    token->entityMap = NULL;
    token->entities = NULL;
    token->components = NULL;
}
eOCT_pool* eOCT_context_getComponentPool(OCT_local contextHandle, eOCT_componentDescription component) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
    return iOCT_context_getComponentPool(context, component.componentTypeIndex_reg);
}

void eOCT_entityContext_prepare(OCT_global contextHandle) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
    iOCT_eventManager_clear(&context->events);
}

