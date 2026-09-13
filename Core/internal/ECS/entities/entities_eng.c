#include "entities_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"

OCT_local eOCT_entity_getHandle(OCT_local context, OCT_ID entityID) {
    OCT_local entityHandle = {
        .containerID = context.objectID,
        .objectID = entityID,
    };
    return entityHandle;
}

bool eOCT_entity_isRoot(OCT_local entity) {
    iOCT_entityMeta* entityMeta = eOCT_component_get(entity, iOCT_ECS_inst.entityMetaKey);

    return entityMeta->isRoot;
}


