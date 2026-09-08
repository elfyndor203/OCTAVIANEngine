#include "distance2D_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <box2d/box2d.h>

#include "physicsSystem_int.h"
#include "physics2D/physics2D_int.h"

OCT_local OCT_distance2D_constrain(OCT_local entityA, OCT_local entityB, OCT_vec2 anchorA, OCT_vec2 anchorB, float distance) {
    iOCT_physics2D_b2* physA = iOCT_physics2D_b2_get(entityA);
    iOCT_physics2D_b2* physB = iOCT_physics2D_b2_get(entityB);
    b2WorldId worldID = *(b2WorldId*)eOCT_single_getLocal(iOCT_physicsSystem_inst.box2DWorldKey, entityA.contextHandle);

    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.bodyIdA = physA->b2dBodyID;
    jointDef.bodyIdB = physB->b2dBodyID;
    jointDef.localAnchorA = iOCT_toB2Vec2(OCT_vec2_div(anchorA, iOCT_physicsSystem_inst.unitsPerB2Meter));
    jointDef.localAnchorB = iOCT_toB2Vec2(OCT_vec2_div(anchorB, iOCT_physicsSystem_inst.unitsPerB2Meter));
    jointDef.length = distance / iOCT_physicsSystem_inst.unitsPerB2Meter;
    jointDef.collideConnected = true;

    b2JointId jointID = b2CreateDistanceJoint(worldID, &jointDef);

    iOCT_distance2D distance2D = {
        .jointID = jointID
    };
    OCT_local distanceHandle = {
        .contextHandle = entityA.contextHandle,
        .containerID = OCT_ID_NULL
    };
    eOCT_mappedPool* distancePool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, entityA.contextHandle);
    eOCT_mappedPool_addEntry(distancePool, &distance2D, &distanceHandle.objectID, NULL);
    return distanceHandle;
}

OCT_local OCT_rope2D_constrain(OCT_local entityA, OCT_local entityB, OCT_vec2 anchorA, OCT_vec2 anchorB, float length) {
    iOCT_physics2D_b2* physA = iOCT_physics2D_b2_get(entityA);
    iOCT_physics2D_b2* physB = iOCT_physics2D_b2_get(entityB);
    b2WorldId worldID = *(b2WorldId*)eOCT_single_getLocal(iOCT_physicsSystem_inst.box2DWorldKey, entityA.contextHandle);

    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.bodyIdA = physA->b2dBodyID;
    jointDef.bodyIdB = physB->b2dBodyID;
    jointDef.localAnchorA = iOCT_toB2Vec2(OCT_vec2_div(anchorA, iOCT_physicsSystem_inst.unitsPerB2Meter));
    jointDef.localAnchorB = iOCT_toB2Vec2(OCT_vec2_div(anchorB, iOCT_physicsSystem_inst.unitsPerB2Meter));
    jointDef.minLength = 0;
    jointDef.maxLength = length / iOCT_physicsSystem_inst.unitsPerB2Meter;
    jointDef.length = length / iOCT_physicsSystem_inst.unitsPerB2Meter;
    jointDef.enableLimit = true;
    jointDef.enableSpring = true;
    jointDef.hertz = 0;
    jointDef.dampingRatio = 0.99f;
    jointDef.collideConnected = true;

    b2JointId jointID = b2CreateDistanceJoint(worldID, &jointDef);

    iOCT_distance2D distance2D = {
        .jointID = jointID
    };
    OCT_local ropeHandle = {
        .contextHandle = entityA.contextHandle,
        .containerID = OCT_ID_NULL
    };
    eOCT_mappedPool* distancePool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, entityA.contextHandle);
    eOCT_mappedPool_addEntry(distancePool, &distance2D, &ropeHandle.objectID, NULL);
    return ropeHandle;
}
