#include "constraints_int.h"
#include "types_int.h"

#include "physicsSystem_int.h"

OCT_local OCT_hitbox2D_new_OLD(OCT_local entity, OCT_vec2 dimensions, OCT_vec2 position, float rotation) {
    if (!eOCT_entity_hasComponent(entity, iOCT_physicsSystem_inst.physics2DKey, NULL)) {
        OCT_ERROR_LOG(OCT_EXIT_REQUIREMENT_NOT_MET, "Entities must have physics components attached");
        return OCT_LOCAL_NULL;
    }

    iOCT_hitbox2D newBox = {
        .entity = entity,
        .dimensions = dimensions,
        .position = position,
        .rotation = rotation,
        .enabled = true
    };
    eOCT_mappedPool* boxMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.collider2DKey, entity.contextHandle);
    eOCT_mappedPool_addEntry(boxMPool, &newBox, &newBox.hitboxID, NULL);
    OCT_local newHandle = {
        .contextHandle = entity.contextHandle,
        .objectID = newBox.hitboxID
    };

    iOCT_physics2D_oct* phys = eOCT_entity_getComponent(newBox.entity, iOCT_physicsSystem_inst.physics2DKey);
    phys->inertia += phys->mass + (dimensions.x * dimensions.x) + (dimensions.y * dimensions.y) / 12.0f;

    return newHandle;
}

bool iOCT_hitbox2D_solve(iOCT_hitbox2D hitboxA, iOCT_hitbox2D hitboxB) {
    if (!hitboxA.enabled || !hitboxB.enabled) {
        return false;
    }

    iOCT_physics2D_oct* physA = eOCT_entity_getComponent(hitboxA.entity, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D_oct* physB = eOCT_entity_getComponent(hitboxB.entity, iOCT_physicsSystem_inst.physics2DKey);

    OCT_vec2* posA = (OCT_vec2*)eOCT_entity_getFieldOnce(hitboxA.entity, iOCT_physicsSystem_inst.position2DTicket);
    OCT_vec2* posB = (OCT_vec2*)eOCT_entity_getFieldOnce(hitboxB.entity, iOCT_physicsSystem_inst.position2DTicket);
    float* rotationA = (float*)eOCT_entity_getFieldOnce(hitboxA.entity, iOCT_physicsSystem_inst.rotationTicket);
    float* rotationB = (float*)eOCT_entity_getFieldOnce(hitboxA.entity, iOCT_physicsSystem_inst.rotationTicket);
    OCT_mat3* transformA = iOCT_globalMatrix2D_get(hitboxA.entity);
    OCT_mat3* transformB = iOCT_globalMatrix2D_get(hitboxB.entity);

    // Radius check
    OCT_vec2 hitboxAGlobal = OCT_vec2_add(OCT_mat3_getTranslation(*transformA), hitboxA.position);
    OCT_vec2 hitboxBGlobal = OCT_vec2_add(OCT_mat3_getTranslation(*transformB), hitboxB.position);
    float distance = OCT_vec2_mag(OCT_vec2_sub(hitboxAGlobal, hitboxBGlobal));
    float radiusA = OCT_vec2_mag(OCT_vec2_div(hitboxA.dimensions, 2));
    float radiusB = OCT_vec2_mag(OCT_vec2_div(hitboxB.dimensions, 2));
    if (distance > radiusA + radiusB) {
        return false;
    }

    // SAT check
    OCT_rect2 rectA = {
        .dimensions = OCT_vec2_mul_eleWise(hitboxA.dimensions, OCT_mat3_getScale(*transformA)),
        .center = hitboxAGlobal,
        .rotationRad = hitboxA.rotation + OCT_mat3_getRotation(*transformA)
    };
    OCT_rect2 rectB = {
        .dimensions = OCT_vec2_mul_eleWise(hitboxB.dimensions, OCT_mat3_getScale(*transformB)),
        .center = hitboxBGlobal,
        .rotationRad = hitboxB.rotation + OCT_mat3_getRotation(*transformB)
    };
    OCT_vec2 sourceAxis;
    float overlap;
    OCT_AorB referenceRect;
    OCT_vec2 MTV = OCT_rect2_SAT(rectA, rectB, &sourceAxis, &overlap, &referenceRect);

    if (OCT_vec2_equal(MTV, OCT_VEC2_ZERO, OCT_FLOAT_EPSILON)) {
        return false;
    }

    OCT_vec2 contactPoint1;
    OCT_vec2 contactPoint2;
    OCT_AorB validContacts;
    if (referenceRect == OCT_A) {
        validContacts = OCT_rect2_contacts(rectA, rectB, MTV, sourceAxis, overlap, &contactPoint1, &contactPoint2);
    } else {
        validContacts = OCT_rect2_contacts(rectB, rectA, MTV, sourceAxis, overlap, &contactPoint1, &contactPoint2);
    }
    OCT_vec2 contactToUse;
    if (validContacts == OCT_NEITHER) {
        return false;
    }
    if (validContacts == OCT_A) {
        contactToUse = contactPoint1;
    } else if (validContacts == OCT_B) {
        contactToUse = contactPoint2;
    } else {
        contactToUse = OCT_vec2_div(OCT_vec2_add(contactPoint1, contactPoint2), 2.0f);
    }

    OCT_vec2 correctionPosA = OCT_VEC2_ZERO;
    OCT_vec2 correctionPosB = OCT_VEC2_ZERO;
    OCT_vec2 correctionVelA = OCT_VEC2_ZERO;
    OCT_vec2 correctionVelB = OCT_VEC2_ZERO;
    float correctionAngVelA = 0.0f;
    float correctionAngVelB = 0.0f;
    float invMassA = 1 / physA->mass;
    float invMassB = 1 / physB->mass;
    float invInertiaA = 1 / physA->inertia;
    float invInertiaB = 1 / physB->inertia;
    bool correctVel = true;
    OCT_AorB correctStyle = OCT_BOTH;
    if (physA->fixed) {
        correctStyle = OCT_AorB_makeFalse(correctStyle, OCT_A);
        invMassA = 0;
        invInertiaA = 0;
    }
    if (physB->fixed) {
        correctStyle = OCT_AorB_makeFalse(correctStyle, OCT_B);
        invMassB = 0;
        invInertiaB = 0;
    }
    float totalInvMass = invMassA + invMassB;

    OCT_vec2 normal = OCT_vec2_unit(MTV);
    OCT_vec2 rVecA = OCT_vec2_sub(contactToUse, hitboxAGlobal);
    OCT_vec2 rVecB = OCT_vec2_sub(contactToUse, hitboxBGlobal);
    OCT_vec2 velA_contact = OCT_vec2_add(physA->velocity, OCT_vec2_mul(OCT_vec2_perp(rVecA, OCT_A), physA->angVelocity));
    OCT_vec2 velB_contact = OCT_vec2_add(physB->velocity, OCT_vec2_mul(OCT_vec2_perp(rVecB, OCT_A), physB->angVelocity));
    OCT_vec2 relativeVel = OCT_vec2_sub(velB_contact, velA_contact);

    float velocityAlongNormal = OCT_vec2_dot(relativeVel, normal);
    float rACrossN = OCT_vec2_cross(rVecA, normal);
    float rBCrossN = OCT_vec2_cross(rVecB, normal);
    float angularTermA = (rACrossN * rACrossN) * invInertiaA;
    float angularTermB = (rBCrossN * rBCrossN) * invInertiaB;
    float denom = totalInvMass + angularTermA + angularTermB;
    // restitution __ NOTE __
    float impulseValue = (-1) * velocityAlongNormal / denom;
    OCT_vec2 impulse = OCT_vec2_mul(normal, impulseValue);
    if (velocityAlongNormal > 0) {
        correctVel = false;
    }
    if (correctStyle == OCT_A) {
        correctionPosA = OCT_vec2_neg(MTV);
        correctionVelA = OCT_vec2_mul(impulse, (-1) * invMassA * correctVel);
        correctionAngVelA = (-1) * rACrossN * impulseValue * invInertiaA;

    } else if (correctStyle == OCT_B) {
        correctionPosB = MTV;
        correctionVelB = OCT_vec2_mul(impulse,  invMassB * correctVel);
        correctionAngVelB = rBCrossN * impulseValue * invInertiaB;
    }
    else if (correctStyle == OCT_BOTH) {
        correctionVelA = OCT_vec2_mul(impulse, (-1) * invMassA * correctVel);
        correctionVelB = OCT_vec2_mul(impulse, invMassB * correctVel);

        correctionPosA = OCT_vec2_mul(OCT_vec2_neg(MTV), invMassA / totalInvMass);
        correctionPosB = OCT_vec2_mul(MTV, invMassB / totalInvMass);

        correctionAngVelA = (-1) * rACrossN * impulseValue * invInertiaA;
        correctionAngVelB = rBCrossN * impulseValue * invInertiaB;
    }
    else {
        return false;
    }

    correctionPosA = OCT_vec2_mul(correctionPosA, 0.9);
    correctionPosB = OCT_vec2_mul(correctionPosB, 0.9);
    *posA = OCT_vec2_add(*posA, correctionPosA);
    *posB = OCT_vec2_add(*posB, correctionPosB);
    *transformA = OCT_mat3_translateTo(*transformA, *posA);
    *transformB = OCT_mat3_translateTo(*transformB, *posB);
    physA->velocity = OCT_vec2_add(physA->velocity, correctionVelA);
    physB->velocity = OCT_vec2_add(physB->velocity, correctionVelB);

    physA->angVelocity += correctionAngVelA;
    physB->angVelocity += correctionAngVelB;
    *transformA = OCT_mat3_rotate(*transformA, physA->angVelocity * iOCT_physicsSystem_inst.dt);
    *transformB = OCT_mat3_rotate(*transformB, physB->angVelocity * iOCT_physicsSystem_inst.dt);
    *rotationA += physA->angVelocity * iOCT_physicsSystem_inst.dt;
    *rotationB += physB->angVelocity * iOCT_physicsSystem_inst.dt;
    return true;
}
