#include "collisions_int.h"

#include "OCT_Core_eng.h"

OCT_local OCT_collisionWatch_new(OCT_local colliderA, OCT_local colliderB) {
    if (!OCT_global_isEqual(colliderA.contextHandle, colliderB.contextHandle)) {
        OCT_ERROR_LOG(OCT_EXIT_SOURCE_MISMATCH, "Colliders are from different contexts");
    }
    iOCT_collisionWatch newWatch = {
        .colliderA = colliderA,
        .colliderB = colliderB,
        .pairValue = iOCT_pairIDs(OCT_MAX(colliderA.objectID, colliderB.objectID), OCT_MIN(colliderA.objectID, colliderB.objectID)),
        .state = OCT_COLLISION_NONE
    };
    OCT_local watchHandle = {
        .contextHandle = colliderA.contextHandle,
        .containerID = OCT_ID_NULL
    };

    iOCT_collisionWatch_new(colliderA.contextHandle, &newWatch, &watchHandle.objectID, NULL);
    return watchHandle;
}