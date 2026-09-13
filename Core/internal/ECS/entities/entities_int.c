#include "entities_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "registry/registry_int.h"
#include "ECS/dataPatterns/components/components_int.h"

OCT_local iOCT_entity_new(iOCT_entityContext* context) {
	OCT_index newIndex;
	// eOCT_pool_addEntryOld(&context->entityPool, &newIndex);
	eOCT_pool_addEntryNew(&context->entities, NULL, &newIndex);
	OCT_ID newID = eOCT_IDMap_register(&context->entityIDMap, newIndex);

	return (OCT_local) {
		.objectID = newID,
		.containerID = context->contextID,
		.contextHandle = {
		.systemID = OCT_ID_ECS,
		.objectID = context->contextID}
	};
}

OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex) {
	OCT_index* array = (OCT_index*)context->entities.array;
	return &array[entityIndex * iOCT_registry_inst.components.count];
}

void iOCT_entity_attachMeta(OCT_local entity) {
	iOCT_entityMeta metadata = {
		.componentsAttached = 0,
		.componentsEnabled = 0,
		.isRoot = false,
		.entity = entity
	};
	eOCT_component_attach(entity, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);
}

void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentKey component, OCT_index skip) {
	for (OCT_index compIndex = 0; compIndex < componentPool->count; compIndex++) {
		if (compIndex == skip) {	// for when creating a new entry, the ID will already be resolved by itself later
			continue;
		}
		OCT_local* entity = (OCT_local*)eOCT_pool_access(componentPool, compIndex, component.entityHandleValueOffset);
		OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity->objectID);
		OCT_index* componentSlot = iOCT_component_getEntitySlot(context, entityIndex, component);

		// if (*componentSlot != compIndex) {
		// 	printf("Index updated. Old index: %zu\n", *componentSlot);
		// }
		// printf("Entity %zu now has component of type %zu at index %zu\n", entity.objectID, component.componentTypeIndex, compIndex);
		*componentSlot = compIndex;
	}
}

void iOCT_entity_updateAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB attachOrDetach) {
	assert(componentIndex >= 0 && componentIndex < 64);

	if (!OCT_AorB_one(attachOrDetach)) {
		OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Choose attach or detach");
	}
	uint64_t* mask = &entityMeta->componentsAttached;
	if (attachOrDetach == OCT_A) {
		*mask |= (1ULL << componentIndex);
	} else {
		*mask &= ~(1ULL << componentIndex);
	}
}
void iOCT_entity_updateEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB enableOrDisable) {
	assert(componentIndex >= 0 && componentIndex < 64);

	if (!OCT_AorB_one(enableOrDisable)) {
		OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Choose enable or disable");
	}
	uint64_t* mask = &entityMeta->componentsEnabled;
	if (enableOrDisable == OCT_A) {
		*mask |= (1ULL << componentIndex);
	} else {
		*mask &= ~(1ULL << componentIndex);
	}
}

bool iOCT_entity_readAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex) {
	uint64_t* mask = &entityMeta->componentsAttached;
	if (*mask & (1ULL << componentIndex)) {
		return true;
	}
	return false;
}
bool iOCT_entity_readEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex) {
	uint64_t* mask = &entityMeta->componentsEnabled;
	if (*mask & (1ULL << componentIndex)) {
		return true;
	}
	return false;
}

void iOCT_entity_attachRootMeta(OCT_local entity) {
	iOCT_entityMeta metadata = {
		.componentsAttached = 0,
		.componentsEnabled = 0,
		.isRoot = true,
		.entity = entity
	};
	eOCT_component_attach(entity, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);
}

