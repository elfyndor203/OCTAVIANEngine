#include "entity_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "entityContext_int.h"
#include "registry/registry_int.h"

/*!
 * Gets a pointer to the start of a given entity in the form of the first component slot
 * @param context
 * @param entityIndex
 * @return entityBasePtr
 */
static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);
/*!
 * Returns the slot in the entity where the given component's index is stored.
 * @param context
 * @param entityIndex
 * @param component
 * @return componentSlotPtr
 */
static OCT_index* iOCT_entity_getComponentIndexEntry(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component);
/*!
 * Re-resolves all indices stored in entities. Run after any component pool shuffling.
 * @param context
 * @param componentPool
 * @param component
 */
static void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentKey component, OCT_index skip);

void iOCT_entity_updateAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB attachOrDetach);
void iOCT_entity_updateEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB enableOrDisable);
bool iOCT_entity_readAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex);
bool iOCT_entity_readEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex);

#pragma region basics
OCT_local OCT_entity_new(OCT_global contextHandle) {
	//printf("Context ID: %zu\n", contextHandle.objectID);
	// iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextHandle.objectID);
	//printf("Context: %p\n", context);
	OCT_local entityHandle = iOCT_entity_new(context);
	entityHandle.contextHandle = contextHandle;

	iOCT_entityMeta metadata = {
		.componentsAttached = 0,
		.componentsEnabled = 0,
		.entity = entityHandle,
		.isRoot = false
	};
	eOCT_entity_attachComponent(entityHandle, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);
	return entityHandle;
}
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

void* eOCT_entity_attachComponent(OCT_local entity, eOCT_componentKey componentKey, void* source, OCT_index* outIndex) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	OCT_index* entityComponentEntry = iOCT_entity_getComponentIndexEntry(context, entityIndex, componentKey);
	eOCT_pool* componentPool = iOCT_context_getComponentPool(context, componentKey.componentTypeIndex);
	OCT_index destinationIndex;

	void* dataLoc = eOCT_pool_addEntryNew(componentPool, source, &destinationIndex);
	iOCT_entity_resolveIndices(context, componentPool, componentKey, destinationIndex);
	*entityComponentEntry = destinationIndex;

	if (outIndex) {
		*outIndex = destinationIndex;
	}

	iOCT_entityMeta* entityMeta = iOCT_entity_getComponent(context, entityIndex, iOCT_ECS_inst.entityMetaKey.componentTypeIndex);

	iOCT_entity_updateAttachedMask(entityMeta, componentKey.componentTypeIndex, OCT_A);
	iOCT_entity_updateEnabledMask(entityMeta, componentKey.componentTypeIndex, OCT_A);
	return dataLoc;
}

void iOCT_entity_attachMeta(OCT_local entity) {
	iOCT_entityMeta metadata = {
		.componentsAttached = 0,
		.componentsEnabled = 0,
		.isRoot = false,
		.entity = entity
	};
	eOCT_entity_attachComponent(entity, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);
}
#pragma endregion

#pragma region accessors

void* eOCT_entity_getComponent(OCT_local entity, eOCT_componentKey component) {
	if (!eOCT_entity_hasComponent(entity, component, NULL)) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Entity does not have this component attached");
	}
	iOCT_entityContext* context = eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, entity.containerID);

	if (!context) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
	}
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	void* dataLoc = iOCT_entity_getComponent(context, entityIndex, component.componentTypeIndex);
	if (!dataLoc) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
	}
	return dataLoc;
}
// void* eOCT_entity_getComponent(eOCT_contextToken contextToken, OCT_local entity, eOCT_componentKey component) {
// 	iOCT_entityContext* context = contextToken.contextPtr;
// 	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);
// 	void* dataLoc = iOCT_entity_getComponent(context, entityIndex, component.componentTypeIndex);
//
// 	if (!dataLoc) {
// 		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Failed to get component");
// 	}
// 	return dataLoc;
// }

void* eOCT_entity_getFieldByToken(eOCT_contextToken contextToken, OCT_local entity, eOCT_fieldTicket field) {
	if (!contextToken.valid) {
		OCT_ERROR_LOG(OCT_EXIT_STALE_REFERENCE, "Context token invalid");
	}
	OCT_index entityIndex = eOCT_IDMap_getIndex(contextToken.entityMap, entity.objectID);
	if (entityIndex == OCT_INDEX_NULL) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
	}

	OCT_index* entityBase = iOCT_entity_get((iOCT_entityContext*)contextToken.contextPtr, entityIndex);
	OCT_index componentIndex = *(OCT_index*)(entityBase + field.providerTypeIndex);

	eOCT_pool* componentPool = (eOCT_pool*)eOCT_pool_access(contextToken.components, field.providerTypeIndex, 0);
	void* fieldLoc = eOCT_pool_access(componentPool, componentIndex, field.offsetFromStruct);

	return fieldLoc;
} // multi use access 
void* eOCT_entity_getField(OCT_local entity, eOCT_fieldTicket field) {
	iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index componentIndex = *(entityBase + field.providerTypeIndex);

	eOCT_pool* componentPool = (eOCT_pool*)eOCT_pool_access(&context->components, field.providerTypeIndex, 0);
	void* fieldLoc = eOCT_pool_access(componentPool, componentIndex, field.offsetFromStruct);

	return fieldLoc;
} // single use access
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {	// actual access logic
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* componentIndexBase = entityBase + componentTypeIndex;
	OCT_index componentIndex = *componentIndexBase;

	// printf("Component index: %zu\n", componentIndex);

	if (componentIndex == OCT_INDEX_NULL) {
		return NULL;
	}

	eOCT_pool* componentPool = iOCT_context_getComponentPool(context, componentTypeIndex);
	void* dataLoc = eOCT_pool_access(componentPool, componentIndex, 0);
	return dataLoc;
}

#pragma endregion

#pragma region utils
bool OCT_entity_sameContext(OCT_local entity1, OCT_local entity2) {
	if (entity1.containerID == entity2.containerID) {
		return true;
	}
	else {
		return false;
	}
}

bool OCT_entity_fromContext(OCT_local entity, OCT_local context) {
	if (entity.containerID == context.objectID) {
		return true;
	}
	else {
		return false;
	}
}

OCT_local eOCT_entity_getHandle(OCT_local context, OCT_ID entityID) {
	OCT_local entityHandle = {
		.containerID = context.objectID,
		.objectID = entityID,
	};
	return entityHandle;
}
// OCT_global eOCT_entity_getContextHandle(OCT_local entity) {
// 	if (entity.objectID == OCT_ID_NULL) {
// 		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
// 	}
// 	return entity.contextHandle;
// }

bool eOCT_entity_hasComponent(OCT_local entity, eOCT_componentKey component, bool* enabledOut) {
	iOCT_entityContext* context = iOCT_entityContext_get(entity.contextHandle.objectID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	iOCT_entityMeta* entityMeta = iOCT_entity_getComponent(context, entityIndex, iOCT_ECS_inst.entityMetaKey.componentTypeIndex);

	bool attached;
	if (iOCT_entity_readAttachedMask(entityMeta, component.componentTypeIndex)) {
		attached = true;
	} else {
		attached = false;
	}

	bool enabled;
	if (iOCT_entity_readEnabledMask(entityMeta, component.componentTypeIndex)) {
		enabled = true;
	} else {
		enabled = false;
	}

	if (enabledOut) {
		*enabledOut = enabled;
	}
	return attached;
}

bool eOCT_entity_isRoot(OCT_local entity) {
	iOCT_entityMeta* entityMeta = eOCT_entity_getComponent(entity, iOCT_ECS_inst.entityMetaKey);

	return entityMeta->isRoot;
}

bool eOCT_entity_hasExternalComponent(OCT_local entity);
#pragma endregion

#pragma region statics
static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex) {
	OCT_index* array = (OCT_index*)context->entities.array;
	return &array[entityIndex * iOCT_registry_inst.components.count];
}

static OCT_index* iOCT_entity_getComponentIndexEntry(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component) { // __NOTE__ copied into eOCT_entity_getField, maybe fix
	if (!context) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
	}
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* componentSlot = entityBase + component.componentTypeIndex;

	return componentSlot;
}
static void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentKey component, OCT_index skip) {
	for (OCT_index compIndex = 0; compIndex < componentPool->count; compIndex++) {
		if (compIndex == skip) {	// for when creating a new entry, the ID will already be resolved by itself later
			continue;
		}
		OCT_local* entity = (OCT_local*)eOCT_pool_access(componentPool, compIndex, component.entityHandleValueOffset);
		OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity->objectID);
		OCT_index* componentSlot = iOCT_entity_getComponentIndexEntry(context, entityIndex, component);

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

#pragma endregion

void OCT_entity_printAllComponentLinks(OCT_global contextHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

	OCT_index* entityArray = (OCT_index*)context->entities.array;

	for (OCT_index entityCtr = 0; entityCtr < context->entities.count; entityCtr++) {
		OCT_index* entityBase = entityArray + entityCtr * context->components.count;
		printf("Entity #%zu:\n", entityCtr);
		for (OCT_index componentTypeCtr = 0; componentTypeCtr < context->components.count; componentTypeCtr++) {
			OCT_index* componentIndexBase = entityBase + componentTypeCtr;
			printf("  Component type %zu: Index: %zu\n", componentTypeCtr, *componentIndexBase);
		}

	}
}
