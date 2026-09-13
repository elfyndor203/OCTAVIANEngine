#include "OCT_Core_eng.h"

#include "ECS/ECS_int.h"
#include "../internal/ECS/entities/entities_int.h"

void init_system_register_CORE() {
    eOCT_componentDescription entityMeta = {
        .name = "entityMeta",
        .entityHandleValueOffset = offsetof(iOCT_entityMeta, entity),
        .keyCacheLocation = &iOCT_ECS_inst.entityMetaKey,
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,
        .sort = false,
        .stride = sizeof(iOCT_entityMeta)
    };

    eOCT_systemDescription systemDescription = {
        .name = "OCTAVIANEngine CORE",
        .contextInitFx = NULL,
        .systemInitFx = NULL,
        .providedComponents = eOCT_generateComponentDescriptionPool(1, entityMeta, eOCT_END_COMPONENTS),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
    };

    eOCT_registry_registerSystem(systemDescription);
}