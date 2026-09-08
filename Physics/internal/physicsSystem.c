#include "physicsSystem_int.h"

#include <box2d/box2d.h>

#include "physics2D/physics2D.h"
#include "physics2D/physics2D_int.h"
#include "constraints/constraints_int.h"

#define iOCT_PHYSICS_CONSTRAINT_SOLVE_ITERATIONS_DEFAULT 10

iOCT_physicsSystem iOCT_physicsSystem_inst = {0};

void OCT_physicsSystem_config(OCT_config_physics config) {
    if (config.unitsPerMeter == OCT_CONFIG_DEFAULT_FLOAT) {
        iOCT_physicsSystem_inst.unitsPerB2Meter = 1.0f;
    }
    else {
        iOCT_physicsSystem_inst.unitsPerB2Meter = (float)config.unitsPerMeter;
    }

    if (OCT_vec2_equal(config.gravity, OCT_VEC2_NULL, 0)) {
        iOCT_physicsSystem_inst.worldGravity = OCT_GRAVITY_DEFAULT;
    }
    else {
        iOCT_physicsSystem_inst.worldGravity = config.gravity;
    }
}
void iOCT_physicsSystem_init() {
    iOCT_physicsSystem_inst.worldGravity = OCT_GRAVITY_DEFAULT;
    iOCT_physicsSystem_inst.dt = 1.0 / OCT_PHYSICS_REFRESH_DEFAULT;
    iOCT_physicsSystem_inst.unitsPerB2Meter = 1;

    iOCT_physicsSystem_inst.constraintSolveIterations = iOCT_PHYSICS_CONSTRAINT_SOLVE_ITERATIONS_DEFAULT;
}

void iOCT_physicsSystem_contextSetup(OCT_global context) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = iOCT_toB2Vec2(OCT_vec2_div(iOCT_physicsSystem_inst.worldGravity, iOCT_physicsSystem_inst.unitsPerB2Meter));
    b2WorldId worldID = b2CreateWorld(&worldDef);
    b2WorldId* worldSingle = (b2WorldId*)eOCT_single_getLocal(iOCT_physicsSystem_inst.box2DWorldKey, context);
    *worldSingle = worldID;

    // b2BodyDef groundBodyDef = b2DefaultBodyDef();
    // groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
    // b2BodyId groundId = b2CreateBody(worldID, &groundBodyDef);
    // b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
    // b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    // b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
}

void eOCT_PHYSICS_update(OCT_global context) {
    b2WorldId worldID = *(b2WorldId*)eOCT_single_getLocal(iOCT_physicsSystem_inst.box2DWorldKey, context);

    b2World_Step(worldID, 1.0f / 60.0f, 4);

    eOCT_pool* physicsPool = eOCT_component_getPool(context, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D_b2* physicsArray = (iOCT_physics2D_b2*)physicsPool->array;
    eOCT_contextToken contextToken = eOCT_context_getToken(context);
    for (OCT_index physCtr = 0; physCtr < physicsPool->count; physCtr++) {
        iOCT_physics2D_b2* physics = &physicsArray[physCtr];

        OCT_vec2* position = (OCT_vec2*)eOCT_entity_getFieldByToken(contextToken, physics->entityHandle, iOCT_physicsSystem_inst.position2DTicket);    // __NOTE__ THESE ARE LOCAL POSITIONS, NOT GLOBAL
        float* rotation = (float*)eOCT_entity_getFieldByToken(contextToken, physics->entityHandle, iOCT_physicsSystem_inst.rotationTicket);
        b2Vec2 newPos = b2Body_GetPosition(physics->b2dBodyID);
        float newRot = b2Rot_GetAngle(b2Body_GetRotation(physics->b2dBodyID));

        *position = OCT_vec2_mul((OCT_vec2){newPos.x, newPos.y}, iOCT_physicsSystem_inst.unitsPerB2Meter);
        *rotation = newRot;
    }
}

// void eOCT_PHYSICS_updateCustomLoop(OCT_global context) {
//     eOCT_pool* physicsPool = eOCT_component_getPool(context, iOCT_physicsSystem_inst.physics2DKey);
//     iOCT_physics2D_oct* physicsArray = (iOCT_physics2D_oct*)physicsPool->array;
//     eOCT_contextToken contextToken = eOCT_context_getToken(context);
//     for (OCT_index physCtr = 0; physCtr < physicsPool->count; physCtr++) {
//         iOCT_physics2D_oct* physics = &physicsArray[physCtr];
//
//         if (physics->fixed) {
//             continue;
//         }
//         OCT_vec2* position = (OCT_vec2*)eOCT_entity_getFieldByToken(contextToken, physics->entityHandle, iOCT_physicsSystem_inst.position2DTicket);    // __NOTE__ THESE ARE LOCAL POSITIONS, NOT GLOBAL
//         float* rotation = (float*)eOCT_entity_getFieldByToken(contextToken, physics->entityHandle, iOCT_physicsSystem_inst.rotationTicket);
//         physics->prevPos = *position;
//         // iOCT_physics2D_integrateEuler(physics, position, rotation, iOCT_physicsSystem_inst.dt);
//     }
//
//     eOCT_pool* ropePool = &eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, context)->pool;
//     iOCT_rope2D* ropeArray = (iOCT_rope2D*)ropePool->array;
//     eOCT_pool* hitboxPool = &eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.collider2DKey, context)->pool;
//     iOCT_hitbox2D* hitboxArray = (iOCT_hitbox2D*)hitboxPool->array;
//     for (OCT_index iteration = 0; iteration < iOCT_physicsSystem_inst.constraintSolveIterations; iteration++) {
//         for (OCT_index ropeCtr = 0; ropeCtr < ropePool->count; ropeCtr++) {
//             iOCT_rope2D rope = ropeArray[ropeCtr];
//             iOCT_rope2D_solve(rope, contextToken);
//         }
//
//         for (OCT_index hitboxCtr = 0; hitboxCtr < hitboxPool->count; hitboxCtr++) {
//             iOCT_hitbox2D hitbox = hitboxArray[hitboxCtr];
//             for (OCT_index compareCtr = hitboxCtr + 1; compareCtr < hitboxPool->count; compareCtr++) {
//                 iOCT_hitbox2D compare = hitboxArray[compareCtr];
//                 // iOCT_hitbox2D_solve(hitbox, compare);
//             }
//         }
//     }
// }


b2Vec2 iOCT_toB2Vec2(OCT_vec2 octVec2) {
    return (b2Vec2){octVec2.x, octVec2.y};
}

OCT_vec2 iOCT_toOCTVec2(b2Vec2 b2Vec2) {
    return (OCT_vec2){b2Vec2.x, b2Vec2.y};
}