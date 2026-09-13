#include "entityContexts_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "registry/registry_int.h"
#include "events/events_int.h"
#include "ECS/entities/entities_int.h"

eOCT_pool iOCT_entityContext_initComponentPools() {
	OCT_index totalComponents = iOCT_registry_inst.components.count;
	eOCT_pool containerPool = eOCT_pool_open(OCT_ID_ECS, totalComponents, sizeof(eOCT_pool));

	for (OCT_index componentCtr = 0; componentCtr < totalComponents; componentCtr++) {
		eOCT_componentDescription* component = (eOCT_componentDescription*)eOCT_pool_access(&iOCT_registry_inst.components, componentCtr, 0);

		// eOCT_pool* newPool = eOCT_pool_addEntryOld(&containerPool, NULL);
		eOCT_pool* newPool = eOCT_pool_addEntryNew(&containerPool, NULL, NULL);
		*newPool = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, component->stride);	// init actual component pool
		if (component->sortValueOffset != eOCT_POOL_SORT_NONE) {
			eOCT_pool_setSort(newPool, component->sortValueOffset);
		}
		//printf("Allocated component %s with size %zu at %p\n", component->name, component->stride, newPool);
	}

	return containerPool;
}

eOCT_pool iOCT_entityContext_initDataPools() {
	OCT_index totalDataPools = iOCT_registry_inst.localDataPools.count;
	eOCT_pool containerPool = eOCT_pool_open(OCT_ID_ECS, totalDataPools, sizeof(eOCT_mappedPool));

	for (OCT_index dataPoolCtr = 0; dataPoolCtr < totalDataPools; dataPoolCtr++) {
		eOCT_dataPoolDescription* dataPool = (eOCT_dataPoolDescription*)eOCT_pool_access(&iOCT_registry_inst.localDataPools, dataPoolCtr, 0);

		eOCT_mappedPool* newMPool = eOCT_pool_addEntryNew(&containerPool, NULL, NULL);
		*newMPool = eOCT_mappedPool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, dataPool->stride, dataPool->elementIDValueOffset);
		if (dataPool->sort) {
			eOCT_pool_setSort(&newMPool->pool, dataPool->sortValueOffset);
		}
	}
	return containerPool;
}

eOCT_pool iOCT_entityContext_initSingles() {
	OCT_index localSinglesCt = iOCT_registry_inst.localSingles.count;

	eOCT_pool singlesPool = eOCT_pool_open(OCT_ID_ECS, localSinglesCt, sizeof(eOCT_dataUnion));
	for (OCT_index singleCtr = 0; singleCtr < localSinglesCt; singleCtr++) {
		eOCT_pool_addEntryNew(&singlesPool, NULL, NULL);
	}

	return singlesPool;
}
OCT_local iOCT_entityContext_initRootEntity(iOCT_entityContext* context) {
	OCT_local rootEntity = iOCT_entity_new(context);
	assert(rootEntity.objectID == iOCT_ENTITY_ROOT_ID);

	iOCT_entityMeta metadata = {
		.componentsAttached = 0,
		.componentsEnabled = 0,
		.isRoot = true,
		.entity = rootEntity
	};
	iOCT_entityMeta* dest = eOCT_component_attach(rootEntity, iOCT_ECS_inst.entityMetaKey, &metadata, NULL);

	const OCT_index componentsTotal = iOCT_registry_inst.components.count;
	for (OCT_index componentCtr = 0; componentCtr < componentsTotal; componentCtr++) {
		eOCT_componentDescription* component = (eOCT_componentDescription*)eOCT_pool_access(&iOCT_registry_inst.components, componentCtr, 0);
		eOCT_rootAttachmentFx attachFx = component->rootAttachmentFx;
		if (attachFx) {
			printf("Attached component %s to ROOT\n", component->name);
			(*attachFx)(rootEntity);
		}
	}

	return rootEntity;
}

void iOCT_entityContext_initSystems(OCT_global contextHandle) {
	eOCT_pool initFxPool = iOCT_registry_inst.contextInitFxs;

	for (OCT_index initFxCtr = 0; initFxCtr < initFxPool.count; initFxCtr++) {
		eOCT_contextInitFx initFx = *(eOCT_contextInitFx*)eOCT_pool_access(&initFxPool, initFxCtr, 0);
		initFx(contextHandle);
	}
}

eOCT_pool* iOCT_context_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex) {
	eOCT_pool* poolsArray = (eOCT_pool*)context->components.array;
	return &poolsArray[componentIndex];
}

iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextID);
	return context;
}