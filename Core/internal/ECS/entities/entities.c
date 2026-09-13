#include "entities_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "registry/registry_int.h"
#include "ECS/dataPatterns/components/components_int.h"

OCT_local OCT_entity_new(OCT_global contextHandle) {
    //printf("Context ID: %zu\n", contextHandle.objectID);
    // iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
    iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextHandle.objectID);
    //printf("Context: %p\n", context);
    OCT_local entityHandle = iOCT_entity_new(context);
    entityHandle.contextHandle = contextHandle;

    iOCT_entityMeta metadata = {
        .componentsAttached = 0,
        .componentsEnabled = 0,
        .entity = entityHandle,
        .isRoot = false
    };
    eOCT_component_attach(entityHandle, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);
    return entityHandle;
}

bool OCT_entity_sameContext(OCT_local entity1, OCT_local entity2) {
    if (entity1.containerID == entity2.containerID) {
        return true;
    }
    else {
        return false;
    }
}

bool OCT_entity_fromContext(OCT_local entity, OCT_local context) {
    if (entity.containerID == context.objectID) {
        return true;
    }
    else {
        return false;
    }
}

void OCT_entity_printAllComponentIndices(OCT_global contextHandle) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    OCT_index* entityArray = (OCT_index*)context->entities.array;

    for (OCT_index entityCtr = 0; entityCtr < context->entities.count; entityCtr++) {
        OCT_index* entityBase = entityArray + entityCtr * context->components.count;
        printf("Entity #%zu:\n", entityCtr);
        for (OCT_index componentTypeCtr = 0; componentTypeCtr < context->components.count; componentTypeCtr++) {
            OCT_index* componentIndexBase = entityBase + componentTypeCtr;
            printf("  Component type %zu: Index: %zu\n", componentTypeCtr, *componentIndexBase);
        }

    }
}
