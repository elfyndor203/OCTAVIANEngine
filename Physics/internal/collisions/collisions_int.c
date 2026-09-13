#include "collisions_int.h"

#include "OCT_Core_eng.h"
#include <box2d/box2d.h>

void iOCT_collisionWatch_update(OCT_global context) {
    eOCT_mappedPool watchPool = *iOCT_collisionWatch_getPool(context);
    iOCT_collisionWatch* watchArray = watchPool.pool.array;

    b2WorldId worldID = *iOCT_box2DWorldID_get(context);
    b2ContactEvents contactEvents = b2World_GetContactEvents(worldID);

    for (OCT_index begins = 0; begins < contactEvents.beginCount; begins++) {
        b2ContactBeginTouchEvent beginTouchEvent = contactEvents.beginEvents[begins];
        OCT_local colliderAHandle = *(OCT_local*)b2Shape_GetUserData(beginTouchEvent.shapeIdA);
        OCT_local colliderBHandle = *(OCT_local*)b2Shape_GetUserData(beginTouchEvent.shapeIdB);

        uint64_t collisionPairValue = iOCT_pairIDs(OCT_MAX(colliderAHandle.objectID, colliderBHandle.objectID), OCT_MIN(colliderAHandle.objectID, colliderBHandle.objectID));
        for (OCT_index watches = 0; watches < watchPool.pool.count; watches++) {
            iOCT_collisionWatch* watch = &watchArray[watches];
            if (collisionPairValue != watch->pairValue) {
                continue;
            }
            watch->state = OCT_COLLISION_COLLIDED;
            watch->updated = true;
        }
    }
}   // keep an array of size max(pairValue). iterate over each watch and put them into that array at index (pairValue). then iterate over all events and check the corresponding slot in the array. or check out triangular number indexing

uint64_t iOCT_pairIDs(OCT_ID ID_x, OCT_ID ID_y) {
    uint64_t result;
    uint64_t x = (uint64_t)ID_x;
    uint64_t y = (uint64_t)ID_y;

    if (ID_x < ID_y) {
        result = (y * y) + x;
    } else {
        result = (x * x) + x + y;
    }
    return result;
}