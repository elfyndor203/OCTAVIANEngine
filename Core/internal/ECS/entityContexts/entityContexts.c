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

OCT_global OCT_entityContext_open(OCT_local* rootOut) {
	OCT_ID newID;
	iOCT_entityContext* newContext = eOCT_mappedPool_addEntry(&iOCT_ECS_inst.contextMPool, NULL, &newID, NULL);

	// init entity pool
	OCT_index entityCapacity = eOCT_POOL_CAPACITY_DEFAULT;
	newContext->entityIDMap = eOCT_IDMap_open(newID, entityCapacity);	//
	newContext->entities = eOCT_pool_open(newID, entityCapacity, iOCT_ECS_inst.entitySize);
	eOCT_pool_fillSetting noComponent = {
		.fillStyle = eOCT_POOL_FILLSTYLE_BYTES,
		.value.valueFill = iOCT_COMPONENT_UNSET
	};
	eOCT_pool_setFill(&newContext->entities, noComponent); 	// mark all component indices as unset

	// init components and root entity
	newContext->components = iOCT_entityContext_initComponentPools();
	newContext->dataPools = iOCT_entityContext_initDataPools();
	newContext->singles = iOCT_entityContext_initSingles();
	newContext->events = iOCT_eventManager_open(newID);
	OCT_local rootEntity = iOCT_entityContext_initRootEntity(newContext);
	if (rootOut) {
		*rootOut = rootEntity;
	}

		// finalize
	OCT_global contextHandle = {
		.systemID = OCT_ID_ECS,
		.objectID = newID,
	};

	iOCT_entityContext_initSystems(contextHandle);
	printf("Allocated entityContext %"PRIu64"\n", newID);
	return contextHandle;
}

OCT_local OCT_entityContext_getRoot(OCT_global context) {
	OCT_local rootHandle = {
		.contextHandle = context,
		.containerID = context.objectID,
		.objectID = iOCT_ENTITY_ROOT_ID
	};
	return rootHandle;
}

void OCT_entityContext_dumpEntityPool(OCT_global contextHandle) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextHandle.objectID);
	eOCT_pool_dump(&context->entities);
}



