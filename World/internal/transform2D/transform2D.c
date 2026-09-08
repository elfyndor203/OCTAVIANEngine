#include "transform2D_int.h"
#include "transform2D/transform2D.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <stdbool.h>

#include "world/world_int.h"

static void iOCT_transform2D_printAll(OCT_global context);

bool OCT_transform2D_attach(OCT_local entity, OCT_local parentEntity) {
	if (entity.containerID != parentEntity.containerID){
		printf("Cannot parent to entity in different context\n");
		return false;
	}
	// iOCT_transform2D parentTransform = *(iOCT_transform2D*)eOCT_entity_getComponent(parentEntity, iOCT_world_inst.transform2DKey);
	iOCT_transform2D parentTransform = *iOCT_transform2D_get(parentEntity);

	OCT_index index;
	iOCT_transform2D transform = {
		.entityHandle = entity,
		.parentEntityHandle = parentEntity,
		.position = OCT_VEC2_ZERO,
		.rotation = 0.0f,
		.scale = (OCT_vec2){1.0f, 1.0f},
		.localMatrix = OCT_mat3_identity,
		.globalMatrix = parentTransform.globalMatrix,
		.depth = parentTransform.depth + 1
	};

	iOCT_transform2D* transformLoc = eOCT_entity_attachComponent(entity, iOCT_world_inst.transform2DKey, &transform, &index);
	printf("\nAttached at index %zu\n", index);
	// iOCT_transform2D* transformLoc = (iOCT_transform2D*)eOCT_entity_attachComponentSorted(entity, iOCT_world_inst.transform2DKey, parentTransform.depth + 1);
	// transformLoc->entityHandle = entity;
	// transformLoc->parentEntityHandle = parentEntity;
	// transformLoc->position = OCT_vec2_zero;
	// transformLoc->rotation = 0.0f;
	// transformLoc->scale = (OCT_vec2){1.0f, 1.0f};
	// transformLoc->localMatrix = OCT_mat3_identity;
	// transformLoc->globalMatrix = parentTransform.globalMatrix;
	// transformLoc->depth = parentTransform.depth + 1;

	// printf("Attached transform with depth %zu\n", transformLoc->depth);

	// iOCT_transform2D_printAll(entity.contextHandle);
	return true;
}

OCT_vec2 OCT_transform2D_moveTo(OCT_local entity, OCT_vec2 destination) {
	iOCT_transform2D* transform = iOCT_transform2D_get(entity);
	OCT_vec2 originalPosition = transform->position;

	iOCT_transform2D* parentTransform = (iOCT_transform2D*)eOCT_entity_getComponent(transform->parentEntityHandle, iOCT_world_inst.transform2DKey);
	transform->position = destination;
	transform->localMatrix = OCT_mat3_translateTo(transform->localMatrix, destination);
	transform->globalMatrix = OCT_mat3_mul(parentTransform->globalMatrix, transform->localMatrix);

	return OCT_vec2_sub(transform->position, originalPosition);
}
OCT_vec2 OCT_transform2D_moveBy(OCT_local entity, OCT_vec2 deltaXY) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DKey);
	iOCT_transform2D* parentTransform = (iOCT_transform2D*)eOCT_entity_getComponent(transform->parentEntityHandle, iOCT_world_inst.transform2DKey);

	transform->position = OCT_vec2_add(transform->position, deltaXY);
	transform->localMatrix = OCT_mat3_translateTo(transform->localMatrix, transform->position);
	transform->globalMatrix = OCT_mat3_mul(parentTransform->globalMatrix, transform->localMatrix);
	return transform->position;
}

float OCT_transform2D_rotateTo(OCT_local entity, float radians) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DKey);
	float originalRotation = transform->rotation;

	transform->rotation = radians;

	transform->localMatrix = OCT_mat3_rotateTo(transform->localMatrix, radians);
	transform->globalMatrix = OCT_mat3_rotateTo(transform->globalMatrix, radians);

	return transform->rotation - originalRotation;
}

OCT_vec2 iOCT_transform2D_setPosition(iOCT_transform2D* transform, OCT_vec2 xy) {
	transform->position = xy;
	return transform->position;
}
OCT_vec2 OCT_transform2D_read(OCT_local entity, float* rotationOut, OCT_vec2* scaleOut) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DKey);

	if (rotationOut) {
		*rotationOut = transform->rotation;
	}
	if (scaleOut) {
		*scaleOut = transform->scale;
	}
	return transform->position;
}

void iOCT_transform2D_generateRoot(OCT_local rootEntity) {
	iOCT_transform2D rootTransform = {
		.entityHandle = rootEntity,
		.parentEntityHandle = rootEntity,
		.position = OCT_VEC2_ZERO,
		.rotation = 0.0f,
		.scale = (OCT_vec2){1.0f, 1.0f},
		.localMatrix = OCT_mat3_identity,
		.globalMatrix = OCT_mat3_identity,
		.depth = iOCT_TRANSFORM_ROOT_DEPTH
	};
	eOCT_entity_attachComponent(rootEntity, iOCT_world_inst.transform2DKey, &rootTransform, NULL);

}

// resolves local and global matrices
void iOCT_transform2D_propagate(OCT_global context) {
	eOCT_pool* transformPool = eOCT_component_getPool(context, iOCT_world_inst.transform2DKey);
	iOCT_transform2D* transformArray = (iOCT_transform2D*)transformPool->array;

	if (!transformPool || !transformArray) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Transform array or pool DNE");
		return;
	}
	if (transformPool->count == 0) {	// no transforms -> guaranteed early return
		return;
	}

	iOCT_transform2D* parent;
	iOCT_transform2D* target;
	for (OCT_index transformCtr = 0; transformCtr < transformPool->count; transformCtr++) {
		target = &transformArray[transformCtr];
		target->localMatrix = OCT_mat3_generate(target->position, target->scale, target->rotation);	// resolve local

		if (target->depth == iOCT_TRANSFORM_ROOT_DEPTH) {
			// no parent to handle
			target->globalMatrix = target->localMatrix;
			continue;
		}
		parent = (iOCT_transform2D*)eOCT_entity_getComponent(target->parentEntityHandle, iOCT_world_inst.transform2DKey);
		target->globalMatrix = OCT_mat3_mul(parent->globalMatrix, target->localMatrix);		// resolve global
	}

	//eOCT_pool_dump(transformPool);
}

static void iOCT_transform2D_printAll(OCT_global context) {
	eOCT_pool* transformPool = eOCT_component_getPool(context, iOCT_world_inst.transform2DKey);
	iOCT_transform2D* transformArray = (iOCT_transform2D*)transformPool->array;

	printf("TRANSFORMS:\n");

	for (OCT_index transformCtr = 0; transformCtr < transformPool->count; transformCtr++) {
		iOCT_transform2D transform = transformArray[transformCtr];

		printf("Transform #%zu:\n", transformCtr);
		printf("Entity:\n");
		OCT_local_print(transform.entityHandle, 4);
		printf("Parent:\n");
		OCT_local_print(transform.parentEntityHandle, 4);
		printf("  Position: %f %f UNITS \n", transform.position.x, transform.position.y);
		printf("  Rotation: %f RADIANS \n", transform.rotation);
		printf("  Scale: %f %f\n", transform.scale.x, transform.scale.y);
		printf("  Depth: %zu\n", transform.depth);
		printf("-----------\n");
	}
}