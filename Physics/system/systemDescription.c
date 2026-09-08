#include "OCT_Core_eng.h"

#include "physics2D/physics2D_int.h"
#include "physicsSystem_int.h"
#include "constraints/types_int.h"
#include "constraints/constraints_int.h"
#include "constraints/collider2D_int.h"
#include "constraints/distance2D_int.h"

void system_register_PHYSICS() {
    eOCT_fieldDescription box2DWorld = {
        .name = "box2DWorld",
        .offset = 0,
        .providerType = eOCT_DATAPATTERN_SINGLE,
        .type = eOCT_TYPE_CUSTOM
    };
    eOCT_singleDescription box2DWorldSingle = {
        .name = "box2DWorld",
        .providedField = box2DWorld,
        .global = false,
        .keyCacheLocation = &iOCT_physicsSystem_inst.box2DWorldKey
    };
    eOCT_componentDescription physics2D = {
        .name = "physics2D",
        .entityHandleValueOffset = offsetof(iOCT_physics2D_b2, entityHandle),
        .providedFields = eOCT_POOL_EMPTY,
        .sort = false,
        .sortValueOffset = eOCT_POOL_SORT_NONE,
        .stride = sizeof(iOCT_physics2D_b2),
        .rootAttachmentFx = NULL,
        .keyCacheLocation = &iOCT_physicsSystem_inst.physics2DKey
    };

    eOCT_dataPoolDescription distance2D = {
        .name = "distance2D",
        .stride = sizeof(iOCT_distance2D),
        .providedFields = eOCT_POOL_EMPTY,
        .elementIDValueOffset = offsetof(iOCT_distance2D, distanceID),
        .keyCacheLocation = &iOCT_physicsSystem_inst.distance2DKey,
        .sort = false,
        .global = false
    };
    eOCT_dataPoolDescription collider2D = {
        .name = "collider2D",
        .stride = sizeof(iOCT_collider2D),
        .providedFields = eOCT_POOL_EMPTY,
        .elementIDValueOffset = offsetof(iOCT_collider2D, colliderID),
        .keyCacheLocation = &iOCT_physicsSystem_inst.collider2DKey,
        .sort = false,
        .global = false
    };
    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.transform2DTicket,
        .type = eOCT_TYPE_MAT3,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };
    eOCT_fieldRequest position2D = {
        .name = "position",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.position2DTicket,
        .type = eOCT_TYPE_VEC2,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };
    eOCT_fieldRequest rotation2D = {
        .name = "rotation",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.rotationTicket,
        .type = eOCT_TYPE_FLOAT32,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };
    eOCT_fieldRequest transformParent = {
        .name = "transformParent",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.transformParentTicket,
        .type = eOCT_TYPE_HANDLE_LOCAL,
        .providerType = eOCT_DATAPATTERN_COMPONENT
    };

    eOCT_systemDescription physicsSystem = {
        .name = "Physics",
        .providedComponents = eOCT_generateComponentDescriptionPool(1, physics2D, eOCT_END_COMPONENTS),
        .providedDataPools = eOCT_generateDataPoolDescriptionPool(2, distance2D, collider2D, eOCT_END_DATAPOOLS),
        .providedEvents = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_generateSingleDescriptionPool(1, box2DWorldSingle, eOCT_END_SINGLES),
        .requestedFields = eOCT_generateFieldRequestPool(4, transform2D, position2D, rotation2D, transformParent, eOCT_END_REQUESTS),
        .contextInitFx = iOCT_physicsSystem_contextSetup,
        .systemInitFx = iOCT_physicsSystem_init
    };

    iOCT_physicsSystem_inst.systemID = eOCT_registry_registerSystem(physicsSystem);
}
