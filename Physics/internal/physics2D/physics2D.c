#include "physics2D_int.h"
#include "types_int.h"
#include "physics2D/physics2D.h"

#include "OCT_Core_eng.h"

#include "physicsSystem_int.h"

static OCT_vec2 iOCT_physics2D_resolveFrameNetForceOld(iOCT_physics2D_oct* physics2D);

// void OCT_physics2D_attachOld(OCT_local entity, float mass, bool fixed) {
//     if (mass <= 0) {
//         OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Entities must have positive mass. Fix in place or adjust gravity instead.");
//     }
//     iOCT_physics2D_oct newPhysics = {
//         .entityHandle = entity,
//         .mass = mass,
//         .gravityStrength = 1,
//         .f_const = OCT_VEC2_ZERO,   // does not include gravity
//         .f_frame = OCT_VEC2_ZERO,
//         .velocity = OCT_VEC2_ZERO,
//         .prevPos = OCT_VEC2_ZERO,
//         .fixed = fixed
//     };
//     eOCT_entity_attachComponent(entity, iOCT_physicsSystem_inst.physics2DKey, &newPhysics, NULL);
//
//     printf("Attached physics2D to entity %zu\n", entity.objectID);
// }

void OCT_physics2D_attachNew(OCT_local entity, float mass, bool dynamic) {
    OCT_local* parentEntity = eOCT_entity_getField(entity, iOCT_physicsSystem_inst.transformParentTicket);
    if (!eOCT_entity_isRoot(*parentEntity)) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Physics can only be applied to children of the ROOT. Further children are treated as part of the same rigid body.");
    }
    OCT_mat3 transform = *iOCT_globalMatrix2D_getField(entity);
    OCT_vec2 position = OCT_mat3_getTranslation(transform);
    float rotation = OCT_mat3_getRotation(transform);
    OCT_vec2 positionMeters = OCT_vec2_div(position, iOCT_physicsSystem_inst.unitsPerB2Meter);

    b2WorldId worldID = *(b2WorldId*)eOCT_single_getLocal(iOCT_physicsSystem_inst.box2DWorldKey, entity.contextHandle);

    b2BodyDef newBodyDef = b2DefaultBodyDef();
    newBodyDef.position = (b2Vec2){positionMeters.x, positionMeters.y};
    newBodyDef.rotation = b2MakeRot(rotation);
    b2BodyId newID;
    if (dynamic) {
        newBodyDef.type = b2_dynamicBody;

        newID = b2CreateBody(worldID, &newBodyDef);

        b2Polygon box = b2MakeBox(0.5f,0.5f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.filter.categoryBits = 0;
        b2CreatePolygonShape(newID, &shapeDef, &box);
    }
    else {
        newID = b2CreateBody(worldID, &newBodyDef);
    }

    iOCT_physics2D_b2 newPhysics = {
        .entityHandle = entity,
        .b2dBodyID = newID,
    };
    iOCT_physics2D_b2* dataLoc = eOCT_entity_attachComponent(entity, iOCT_physicsSystem_inst.physics2DKey, &newPhysics, NULL);

    printf("Attached physics2D to entity %zu\n", entity.objectID);
}

// OCT_vec2 OCT_physics2D_setVelocityOld(OCT_local entity, OCT_vec2 velocity) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     OCT_vec2 oldVelocity = physics->velocity;
//     physics->velocity = velocity;
//     return OCT_vec2_sub(velocity, oldVelocity);
// }

OCT_vec2 OCT_physics2D_setVelocity(OCT_local entity, OCT_vec2 velocity) {
    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(entity);
    b2BodyId b2Body = physics->b2dBodyID;

    b2Vec2 oldVelocityMeters = b2Body_GetLinearVelocity(b2Body);
    OCT_vec2 oldVelocity = OCT_vec2_mul(iOCT_toOCTVec2(oldVelocityMeters), iOCT_physicsSystem_inst.unitsPerB2Meter);

    b2Vec2 newVelocityMeters = iOCT_toB2Vec2(OCT_vec2_div(velocity, iOCT_physicsSystem_inst.unitsPerB2Meter));
    b2Body_SetLinearVelocity(b2Body, newVelocityMeters);

    OCT_vec2 delta = OCT_vec2_sub(velocity, oldVelocity);
    return delta;
}

// OCT_vec2 OCT_physics2D_addImpulseOld(OCT_local entity, OCT_vec2 impulse) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     physics->velocity = OCT_vec2_add(physics->velocity, OCT_vec2_div(impulse, physics->mass));
//     return physics->velocity;
// }

void OCT_physics2D_addImpulse(OCT_local entity, OCT_vec2 impulse) {
    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(entity);
    b2BodyId b2Body = physics->b2dBodyID;

    b2Vec2 impulseMeters = iOCT_toB2Vec2(OCT_vec2_div(impulse, iOCT_physicsSystem_inst.unitsPerB2Meter));
    b2Body_ApplyLinearImpulseToCenter(b2Body, impulseMeters, true);
}

void OCT_physics2D_addForce(OCT_local entity, OCT_vec2 force) {
    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(entity);
    b2BodyId b2Body = physics->b2dBodyID;

    b2Vec2 forceMeters = iOCT_toB2Vec2(OCT_vec2_div(force, iOCT_physicsSystem_inst.unitsPerB2Meter));
    b2Body_ApplyForceToCenter(b2Body, forceMeters, true);
}

void OCT_physics2D_lockRotation(OCT_local entity, float radians) {
    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(entity);
    b2BodyId b2Body = physics->b2dBodyID;

    b2Rot newRotation = b2MakeRot(radians);
    b2Vec2 currentPositionMeters = b2Body_GetPosition(b2Body);
    b2Body_SetTransform(b2Body, currentPositionMeters, newRotation);
    b2Body_SetFixedRotation(b2Body, true);
}

// OCT_vec2 OCT_physics2D_addForceContinuousOld(OCT_local entity, OCT_vec2 force) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     physics->f_const = OCT_vec2_add(physics->f_const, force);
//     return physics->f_const;
// }

// OCT_vec2 OCT_physics2D_addForceInstantaneousOld(OCT_local entity, OCT_vec2 force) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     physics->f_frame = OCT_vec2_add(physics->f_frame, force);
//     return physics->f_frame;
// }

// float OCT_physics2D_setGravityOld(OCT_local entity, float gravityStrength) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     float oldGravity = physics->gravityStrength;
//     physics->gravityStrength = gravityStrength;
//     return gravityStrength - oldGravity;
// }

// OCT_vec2 OCT_physics2D_readOld(OCT_local entity, float* massOut, float* gravityOut, OCT_vec2* netForcesOut) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     if (massOut) {
//         *massOut = physics->mass;
//     }
//     if (gravityOut) {
//         *gravityOut = physics->gravityStrength;
//     }
//     if (netForcesOut) {
//         *netForcesOut = OCT_vec2_add(OCT_vec2_add(physics->f_const, physics->f_frame), OCT_vec2_mul(iOCT_physicsSystem_inst.worldGravity, physics->gravityStrength));
//     }
//     return physics->velocity;
// }
OCT_vec2 OCT_physics2D_read(OCT_local entity) {
    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(entity);

    b2Vec2 velocityMeters = b2Body_GetLinearVelocity(physics->b2dBodyID);
    OCT_vec2 velocity = OCT_vec2_div(iOCT_toOCTVec2(velocityMeters), iOCT_physicsSystem_inst.unitsPerB2Meter);
    return velocity;
}
// OCT_vec2 OCT_physics2D_readImplicitOld(OCT_local entity) {
//     iOCT_physics2D_oct* physics = eOCT_entity_getComponent(entity, iOCT_physicsSystem_inst.physics2DKey);
//
//     OCT_vec2 position = *(OCT_vec2*)eOCT_entity_getFieldOnce(entity, iOCT_physicsSystem_inst.position2DTicket);
//     OCT_vec2 deltaPos = OCT_vec2_sub(position, physics->prevPos);
//
//     return OCT_vec2_div(deltaPos, iOCT_physicsSystem_inst.dt);
// }

// void iOCT_physics2D_integrateEulerOld(iOCT_physics2D_oct* physics2D, OCT_vec2* position, float* rotation, float dt) {
//     OCT_vec2 fNet = iOCT_physics2D_resolveFrameNetForce(physics2D);
//     OCT_vec2 accel_lin = OCT_vec2_div(fNet, physics2D->mass);
//
//     OCT_vec2 deltaVel_lin = OCT_vec2_mul(accel_lin, dt);
//     OCT_vec2 newVel_lin = OCT_vec2_add(deltaVel_lin, physics2D->velocity);
//     OCT_vec2 deltaPos = OCT_vec2_mul(newVel_lin, dt);
//
//     *position = OCT_vec2_add(*position, deltaPos);
//     physics2D->velocity = newVel_lin;
//
//     float deltaRot = physics2D->angVelocity * dt;
//     *rotation += deltaRot;
//     // printf("New velocity: %f, %f\n", physics2D->v_lin.x, physics2D->v_lin.y);
// }

// static OCT_vec2 iOCT_physics2D_resolveFrameNetForceOld(iOCT_physics2D_oct* physics2D) {
//     OCT_vec2 gravity = OCT_vec2_mul(iOCT_physicsSystem_inst.worldGravity, physics2D->gravityStrength * physics2D->mass);
//     OCT_vec2 constWGravity = OCT_vec2_add(gravity, physics2D->f_const);
//     OCT_vec2 fNet = OCT_vec2_add(constWGravity, physics2D->f_frame);
//
//     physics2D->f_frame = OCT_VEC2_ZERO;
//
//     return fNet;
// }
