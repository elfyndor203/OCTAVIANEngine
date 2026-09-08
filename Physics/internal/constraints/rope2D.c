#include "constraints_int.h"

// OCT_local OCT_rope2D_new_OLD(OCT_local entityA, OCT_local entityB, float length) {
//     if (!eOCT_entity_hasComponent(entityA, iOCT_physicsSystem_inst.physics2DKey, NULL) || !eOCT_entity_hasComponent(entityB, iOCT_physicsSystem_inst.physics2DKey, NULL)) {
//         OCT_ERROR_LOG(OCT_EXIT_REQUIREMENT_NOT_MET, "Entities must both have physics components attached");
//         return OCT_LOCAL_NULL;
//     }
//     if (OCT_entity_sameContext(entityA, entityB) == false) {
//         OCT_ERROR_LOG(OCT_EXIT_REQUIREMENT_NOT_MET, "Entities must be from the same context");
//         return OCT_LOCAL_NULL;
//     }
//
//     iOCT_rope2D rope = {
//         .entityA = entityA,
//         .entityB = entityB,
//         .length = length,
//         .enabled = true
//     };
//
//     eOCT_mappedPool* ropeMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, entityA.contextHandle);
//     eOCT_mappedPool_addEntry(ropeMPool, &rope, &rope.ropeID, NULL);
//
//     OCT_local ropeHandle = {
//         .contextHandle = entityA.contextHandle,
//         .objectID = rope.ropeID
//     };
//
//     return ropeHandle;
// }

// void OCT_rope2D_length_OLD(OCT_local rope2D, float newLength) {
//     eOCT_mappedPool* ropeMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, rope2D.contextHandle);
//     iOCT_rope2D* rope = (iOCT_rope2D*)eOCT_mappedPool_getByID(ropeMPool, rope2D.objectID);
//
//     iOCT_physics2D_oct* physA = eOCT_entity_getComponent(rope->entityA, iOCT_physicsSystem_inst.physics2DKey);
//     iOCT_physics2D_oct* physB = eOCT_entity_getComponent(rope->entityB, iOCT_physicsSystem_inst.physics2DKey);
//     OCT_vec2* posA = (OCT_vec2*)eOCT_entity_getField(rope->entityA, iOCT_physicsSystem_inst.position2DTicket);
//     OCT_vec2* posB = (OCT_vec2*)eOCT_entity_getField(rope->entityB, iOCT_physicsSystem_inst.position2DTicket);
//
//     OCT_vec2 toMovePos;
//     OCT_vec2 centerPos;
//     iOCT_physics2D_oct* toMovePhys;
//     iOCT_physics2D_oct* centerPhys;
//     if (physA->fixed) {
//         centerPos = *posA;
//         centerPhys = physA;
//         toMovePos = *posB;
//         toMovePhys = physB;
//     }
//     else {
//         centerPos = *posB;
//         centerPhys = physB;
//         toMovePos = *posA;
//         toMovePhys = physA;
//     }
//
//     OCT_vec2 toMoving = OCT_vec2_sub(toMovePos, centerPos);
//     OCT_vec2 toMovingUnit = OCT_vec2_unit(toMoving);
//     OCT_vec2 tangentUnit = OCT_vec2_rotate(toMovingUnit, OCT_deg2rad(90.0f));
//
//     OCT_vec2 lin_momentum = OCT_vec2_mul(toMovePhys->velocity, toMovePhys->mass);
//     float ang_momentum = OCT_vec2_cross(toMoving, lin_momentum);
//     float newVel = ang_momentum / (toMovePhys->mass * newLength);
//
//     toMovePhys->velocity = OCT_vec2_mul(tangentUnit, newVel);
//     rope->length = newLength;
// }

// bool OCT_rope2D_disable_OLD(OCT_local rope2D) {
//     eOCT_mappedPool* ropeMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, rope2D.contextHandle);
//     iOCT_rope2D* rope = (iOCT_rope2D*)eOCT_mappedPool_getByID(ropeMPool, rope2D.objectID);
//
//     bool changed;
//     if (rope->enabled) {
//         changed = true;
//     }
//     else {
//         changed = false;
//     }
//     rope->enabled = false;
//     return changed;
// }
// bool OCT_rope2D_enable_OLD(OCT_local rope2D) {
//     eOCT_mappedPool* ropeMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.distance2DKey, rope2D.contextHandle);
//     iOCT_rope2D* rope = (iOCT_rope2D*)eOCT_mappedPool_getByID(ropeMPool, rope2D.objectID);
//
//     bool changed;
//     if (rope->enabled) {
//         changed = false;
//     }
//     else {
//         changed = true;
//     }
//     rope->enabled = true;
//     return changed;
// }

// void iOCT_rope2D_solve(iOCT_rope2D rope, eOCT_contextToken contextToken) {
//     if (!rope.enabled) {
//         return;
//     }
//     // iOCT_physics2D* physA = eOCT_entity_getComponent(contextToken, constraint.entityA, iOCT_physicsSystem_inst.physics2DKey);
//     // iOCT_physics2D* physB = eOCT_entity_getComponent(contextToken, constraint.entityB, iOCT_physicsSystem_inst.physics2DKey);
//     iOCT_physics2D_oct* physA = eOCT_entity_getComponent(rope.entityA, iOCT_physicsSystem_inst.physics2DKey);
//     iOCT_physics2D_oct* physB = eOCT_entity_getComponent(rope.entityB, iOCT_physicsSystem_inst.physics2DKey);
//
//     OCT_vec2* posA = (OCT_vec2*)eOCT_entity_getFieldByToken(contextToken, rope.entityA, iOCT_physicsSystem_inst.position2DTicket);
//     OCT_vec2* posB = (OCT_vec2*)eOCT_entity_getFieldByToken(contextToken, rope.entityB, iOCT_physicsSystem_inst.position2DTicket);
//
//     float distance = OCT_vec2_mag(OCT_vec2_sub(*posB, *posA));
//
//     if (distance < rope.length) {
//         return;
//     }
//
//     if (physA->fixed && physB->fixed) {
//         return;
//     }
//
//     if (!physA->fixed && !physB->fixed) {
//         OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Two unfixed rope constraint not yet implemented");
//     }
//
//     OCT_vec2* toMovePos;
//     OCT_vec2* centerPos;
//     iOCT_physics2D_oct* toMovePhys;
//     iOCT_physics2D_oct* centerPhys;
//     if (physA->fixed) {
//         centerPos = posA;
//         centerPhys = physA;
//         toMovePos = posB;
//         toMovePhys = physB;
//     }
//     else {
//         centerPos = posB;
//         centerPhys = physB;
//         toMovePos = posA;
//         toMovePhys = physA;
//     }
//
//     OCT_vec2 fromCenter = OCT_vec2_sub(*toMovePos, *centerPos);
//     OCT_vec2 fromCenterUnit = OCT_vec2_unit(fromCenter);
//     OCT_vec2 correctedPos = OCT_vec2_add(OCT_vec2_mul(fromCenterUnit, rope.length), *centerPos);
//
//     float radialSpeed = OCT_vec2_dot(toMovePhys->velocity, fromCenterUnit);
//
//     if (radialSpeed > 0) {
//         OCT_vec2 correctionVel = OCT_vec2_mul(fromCenterUnit, radialSpeed);
//         toMovePhys->velocity = OCT_vec2_sub(toMovePhys->velocity, correctionVel);
//     }
//     *toMovePos = correctedPos;
// }