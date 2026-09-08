#include "collider2D_int.h"
#include "types_int.h"

#include <box2d/box2d.h>

#include "physicsSystem_int.h"
#include "../../../../cmake-build-debug/_deps/box2d-src/src/joint.h"
#include "physics2D/physics2D_int.h"

OCT_local OCT_collider2D_new(OCT_local entity, OCT_shapeType shape, OCT_vec2 dimensions, OCT_vec2 origin, float radians, float density) {
    b2ShapeDef newShape = b2DefaultShapeDef();
    newShape.density = density; // not scaled

    OCT_mat3 globalTransform = *iOCT_globalMatrix2D_getField(entity);

    // figure out the transform of the target entity, relative to the PHYSICS BODY SOURCE, instead of the root
    bool foundPhysicsSourceEntity = false;
    OCT_local potentialPhysicsSourceEntity = entity;
    OCT_local physicsSourceEntity = OCT_LOCAL_NULL;
    while (!foundPhysicsSourceEntity) {
        OCT_local parent = *(OCT_local*)eOCT_entity_getField(potentialPhysicsSourceEntity, iOCT_physicsSystem_inst.transformParentTicket);
        if (eOCT_entity_isRoot(parent)) {
            foundPhysicsSourceEntity = true;
            physicsSourceEntity = potentialPhysicsSourceEntity;
        } else {
            potentialPhysicsSourceEntity = parent;
        }
    }
    // OCT_mat3 physicsSourceTransform = *iOCT_globalMatrix2D_getField(physicsSourceEntity);
    OCT_mat3 physicsSourceTransform = *iOCT_globalMatrix2D_getField(physicsSourceEntity);
    OCT_mat3 invPhysicsSourceTransform = OCT_mat3_inv(physicsSourceTransform);
    OCT_mat3 targetEntityTransform = *iOCT_globalMatrix2D_getField(entity);
    OCT_mat3 targetToPhysicsSourceTransform = OCT_mat3_mul(invPhysicsSourceTransform, targetEntityTransform);
    OCT_vec2 relativeOrigin = OCT_mat3_getTranslation(targetToPhysicsSourceTransform);
    float relativeRadians = OCT_mat3_getRotation(targetToPhysicsSourceTransform);

    iOCT_physics2D_b2* physics = iOCT_physics2D_b2_get(physicsSourceEntity);
    b2BodyId entityBodyID = physics->b2dBodyID;

    OCT_vec2 dimensionsMeters = OCT_vec2_div(dimensions, iOCT_physicsSystem_inst.unitsPerB2Meter);
    OCT_vec2 originMeters = OCT_vec2_div(relativeOrigin, iOCT_physicsSystem_inst.unitsPerB2Meter);
    printf("Origin: %f %f\n", originMeters.x, originMeters.y);
    b2ShapeId newShapeID;
    switch (shape) {
    case OCT_shapeType_rect2:
        ;
        b2Polygon newPolygon = b2MakeOffsetBox(dimensionsMeters.x / 2, dimensionsMeters.y / 2, (b2Vec2){originMeters.x, originMeters.y}, b2MakeRot(relativeRadians));
        newShapeID = b2CreatePolygonShape(entityBodyID, &newShape, &newPolygon);
        break;
    case OCT_shapeType_circ2:
        if (dimensions.x != dimensions.y) {
            OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Circle x and y dimensions must be equal");
        }
        b2Circle newCircle = {
        .center = (b2Vec2){originMeters.x, originMeters.y},
        .radius = dimensionsMeters.x / 2
        };
        newShapeID = b2CreateCircleShape(entityBodyID, &newShape, &newCircle);
        break;
    case OCT_shapeType_caps2:
        ;
        float toCentersDist = (dimensionsMeters.y - dimensionsMeters.x) / 2;
        OCT_vec2 toCenter1 = OCT_vec2_add(OCT_vec2_rotate((OCT_vec2){0, toCentersDist}, relativeRadians), originMeters);
        OCT_vec2 toCenter2 = OCT_vec2_add(OCT_vec2_rotate((OCT_vec2){0, -toCentersDist}, relativeRadians), originMeters);

        b2Capsule newCapsule = {
        .radius = dimensionsMeters.x / 2,
        .center1 = iOCT_toB2Vec2(toCenter1),
        .center2 = iOCT_toB2Vec2(toCenter2)
        };
        newShapeID = b2CreateCapsuleShape(entityBodyID, &newShape, &newCapsule);
        break;
    default:
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "No valid shape provided");
    }

    iOCT_collider2D newCollider = {
        .b2ShapeID = newShapeID,
        .dimensions = dimensions,
        .origin = origin,
        .rotation = radians,
        .shape = shape
    };
    OCT_local colliderHandle = {
        .contextHandle = entity.contextHandle,
        .containerID = OCT_ID_NULL
    };
    eOCT_mappedPool* colliderPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.collider2DKey, entity.contextHandle);
    eOCT_mappedPool_addEntry(colliderPool, &newCollider, &colliderHandle.objectID, NULL);

    return colliderHandle;
}