#include "ECS/dataPatterns/components_eng.h"
#include "ECS/dataPatterns/components/components_int.h"

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
#include "ECS/entities/entities_int.h"

eOCT_pool eOCT_generateComponentDescriptionPool(OCT_index total, eOCT_componentDescription description1, ...) {
    if (total < 1) {
        OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no components are provided");
        return eOCT_POOL_EMPTY;
    }
    va_list args;
    va_start(args, description1);

    eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_componentDescription));
    bool end = false;
    OCT_index processed = 0;
    eOCT_componentDescription newRequest = description1;
    while (!end && processed < total) {
        if (strcmp(newRequest.name, eOCT_END_COMPONENTS.name) == 0) { // checks for END flag
            end = true;

            if (processed != total) {										// END flag should be after all requests are processed
                OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less components provided than expected");
                return pool;
            }
        } else {
            eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
            processed++;
            newRequest = va_arg(args, eOCT_componentDescription);
        }
    }
    eOCT_componentDescription expectedEnd = newRequest;	// most recent: either the END flag or error
    if (strcmp(expectedEnd.name, eOCT_END_COMPONENTS.name) != 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
        return pool;
    }

    va_end(args);
    return pool;
}

eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_pool* sourcePool = &((eOCT_pool*)context->components.array)[componentKey.componentTypeIndex];
    return sourcePool;
}

void* eOCT_component_get(OCT_local entity, eOCT_componentKey component) {
    if (!eOCT_component_isAttached(entity, component, NULL)) {
        OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Entity does not have this component attached");
    }
    iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, entity.containerID);

    if (!context) {
        OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
    }
    OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

    void* dataLoc = iOCT_component_get(context, entityIndex, component.componentTypeIndex);
    if (!dataLoc) {
        OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
    }
    return dataLoc;
}

void* eOCT_component_attach(OCT_local entity, eOCT_componentKey componentKey, void* source, OCT_index* outIndex) {
    // iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
    iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, entity.containerID);
    OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

    OCT_index* entityComponentEntry = iOCT_component_getEntitySlot(context, entityIndex, componentKey);
    eOCT_pool* componentPool = iOCT_context_getComponentPool(context, componentKey.componentTypeIndex);
    OCT_index destinationIndex;

    void* dataLoc = eOCT_pool_addEntryNew(componentPool, source, &destinationIndex);
    iOCT_entity_resolveIndices(context, componentPool, componentKey, destinationIndex);
    *entityComponentEntry = destinationIndex;

    if (outIndex) {
        *outIndex = destinationIndex;
    }

    iOCT_entityMeta* entityMeta = iOCT_component_get(context, entityIndex, iOCT_ECS_inst.entityMetaKey.componentTypeIndex);

    iOCT_entity_updateAttachedMask(entityMeta, componentKey.componentTypeIndex, OCT_A);
    iOCT_entity_updateEnabledMask(entityMeta, componentKey.componentTypeIndex, OCT_A);
    return dataLoc;
}

bool eOCT_component_isAttached(OCT_local entity, eOCT_componentKey component, bool* enabledOut) {
    iOCT_entityContext* context = iOCT_entityContext_get(entity.contextHandle.objectID);
    OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

    iOCT_entityMeta* entityMeta = iOCT_component_get(context, entityIndex, iOCT_ECS_inst.entityMetaKey.componentTypeIndex);

    bool attached;
    if (iOCT_entity_readAttachedMask(entityMeta, component.componentTypeIndex)) {
        attached = true;
    } else {
        attached = false;
    }

    bool enabled;
    if (iOCT_entity_readEnabledMask(entityMeta, component.componentTypeIndex)) {
        enabled = true;
    } else {
        enabled = false;
    }

    if (enabledOut) {
        *enabledOut = enabled;
    }
    return attached;
}

void* eOCT_component_getFieldByToken(eOCT_contextToken contextToken, OCT_local entity, eOCT_fieldTicket field) {
    if (!contextToken.valid) {
        OCT_ERROR_LOG(OCT_EXIT_STALE_REFERENCE, "Context token invalid");
    }
    OCT_index entityIndex = eOCT_IDMap_getIndex(contextToken.entityMap, entity.objectID);
    if (entityIndex == OCT_INDEX_NULL) {
        OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
    }

    OCT_index* entityBase = iOCT_entity_get((iOCT_entityContext*)contextToken.contextPtr, entityIndex);
    OCT_index componentIndex = *(OCT_index*)(entityBase + field.providerTypeIndex);

    eOCT_pool* componentPool = (eOCT_pool*)eOCT_pool_access(contextToken.components, field.providerTypeIndex, 0);
    void* fieldLoc = eOCT_pool_access(componentPool, componentIndex, field.offsetFromStruct);

    return fieldLoc;
} // multi use access
void* eOCT_component_getField(OCT_local entity, eOCT_fieldTicket field) {
    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

    OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
    OCT_index componentIndex = *(entityBase + field.providerTypeIndex);

    eOCT_pool* componentPool = (eOCT_pool*)eOCT_pool_access(&context->components, field.providerTypeIndex, 0);
    void* fieldLoc = eOCT_pool_access(componentPool, componentIndex, field.offsetFromStruct);

    return fieldLoc;
} // single use access
