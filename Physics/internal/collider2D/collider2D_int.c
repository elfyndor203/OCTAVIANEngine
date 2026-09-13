#include "collider2D/collider2D_int.h"
#include "collider2D/collider2D.h"
#include "constraints/types_int.h"

#include <box2d/box2d.h>

#include "physicsSystem_int.h"
#include "physics2D/physics2D_int.h"

void iOCT_collider2D_callWatches(OCT_global context) {
    eOCT_mappedPool colliderPool = *iOCT_collider2D_getPool(context);
    iOCT_collisionWatch* colliderArray = colliderPool.pool.array;

    b2WorldId worldID = *iOCT_box2DWorldID_get(context);
    b2ContactEvents contactEvents = b2World_GetContactEvents(worldID);

    for (OCT_index begins = 0; begins < contactEvents.beginCount; begins++) {
        b2ContactBeginTouchEvent beginTouchEvent = contactEvents.beginEvents[begins];
        OCT_local colliderAHandle = *(OCT_local*)b2Shape_GetUserData(beginTouchEvent.shapeIdA);
        OCT_local colliderBHandle = *(OCT_local*)b2Shape_GetUserData(beginTouchEvent.shapeIdB);
        iOCT_collider2D colliderA = *iOCT_collider2D_get(context, colliderAHandle.objectID);
        iOCT_collider2D colliderB = *iOCT_collider2D_get(context, colliderBHandle.objectID);

        if (colliderA.watchCollision) {
            colliderA.callback(colliderAHandle, colliderBHandle, OCT_COLLISION_COLLIDED);
        }
        if (colliderB.watchCollision) {
            colliderB.callback(colliderBHandle, colliderAHandle, OCT_COLLISION_COLLIDED);
        }
    }


    for (OCT_index ends = 0; ends < contactEvents.endCount; ends++) {
        b2ContactEndTouchEvent endTouchEvent = contactEvents.endEvents[ends];
        OCT_local colliderAHandle = *(OCT_local*)b2Shape_GetUserData(endTouchEvent.shapeIdA);
        OCT_local colliderBHandle = *(OCT_local*)b2Shape_GetUserData(endTouchEvent.shapeIdB);
        iOCT_collider2D colliderA = *iOCT_collider2D_get(context, colliderAHandle.objectID);
        iOCT_collider2D colliderB = *iOCT_collider2D_get(context, colliderBHandle.objectID);

        if (colliderA.watchCollision) {
            colliderA.callback(colliderAHandle, colliderBHandle, OCT_COLLISION_EXITED);
        }
        if (colliderB.watchCollision) {
            colliderB.callback(colliderBHandle, colliderAHandle, OCT_COLLISION_EXITED);
        }
    }
}