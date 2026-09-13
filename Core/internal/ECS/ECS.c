#include "ECS_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "registry/registry_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "events/events_int.h"

// static OCT_index iOCT_ECS_addContextDataPool(eOCT_dataPoolDescription desc);
// static OCT_index iOCT_ECS_addGlobalDataPool(eOCT_dataPoolDescription desc);

iOCT_ECS iOCT_ECS_inst = { 0 };

#pragma region init
void init_OCT_ECS_init() {
	// iOCT_ECS_inst.contextMap = eOCT_IDMap_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT);
	// iOCT_ECS_inst.contextPool = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_entityContext));
	iOCT_ECS_inst.contextMPool = eOCT_mappedPool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_entityContext), offsetof(iOCT_entityContext, contextID));

	// iOCT_ECS_inst.dataPoolSizeAndOrderList = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(size_t));
	//
	// iOCT_ECS_inst.globalDataMap = eOCT_IDMap_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT);
	// iOCT_ECS_inst.globalDataPools = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_pool));

	printf("| ECS initialized\n");
}
void init_OCT_ECS_build() {
	iOCT_ECS_inst.entitySize = iOCT_registry_inst.components.count * sizeof(OCT_index);
}
#pragma endregion

#pragma region pool tracking
// OCT_index iOCT_ECS_addDataPool(eOCT_dataPoolDescription desc, bool global) {
// 	if (global) {
// 		//printf("Added global data pool\n");
// 		return iOCT_ECS_addGlobalDataPool(desc);
// 	}
// 	else {
// 		printf("Added context data pool\n");
// 		return iOCT_ECS_addContextDataPool(desc);
// 	}
// }

// eOCT_pool* eOCT_getDataPool_global(eOCT_dataPoolDescription dataPoolDescription, eOCT_IDMap** IDMapOut) {
// 	if (!dataPoolDescription.global) {
// 		printf("Data pool %s is context local\n", dataPoolDescription.name);
// 		OCT_ERROR_LOG(OCT_EXIT_ACCESS_DENIED, "");
// 		return NULL;
// 	}
// 	if (IDMapOut) {
// 		*IDMapOut = &iOCT_ECS_inst.globalDataMap;
// 	}
//
// 	eOCT_pool* dataPool = (eOCT_pool*)eOCT_pool_access(&iOCT_ECS_inst.globalDataPools, dataPoolDescription.dataPoolTypeIndex_reg, 0);
// 	return dataPool;
// }

// void* eOCT_addGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, bool createID, OCT_ID* IDOut) {
// 	if (!dataPoolDescription.global) {
// 		printf("Data pool %s is context local\n", dataPoolDescription.name);
// 		return NULL;
// 	}
//
// 	eOCT_pool* dataPool = (eOCT_pool*)eOCT_pool_access(&iOCT_ECS_inst.globalDataPools, dataPoolDescription.dataPoolTypeIndex_reg, 0);
// 	eOCT_IDMap* map = &iOCT_ECS_inst.globalDataMap;
//
// 	OCT_index dataIndex;
// 	void* dataLoc = eOCT_pool_addEntry(dataPool, &dataIndex);
//
// 	if (createID && IDOut) {
// 		*IDOut = eOCT_IDMap_register(map, dataIndex);
// 	}
// 	return dataLoc;
// }

// void* eOCT_getGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, OCT_ID dataID) {
// 	if (!dataPoolDescription.global) {
// 		printf("Data pool %s is context local\n", dataPoolDescription.name);
// 		return NULL;
// 	}
//
// 	eOCT_pool* dataPool = (eOCT_pool*)eOCT_pool_access(&iOCT_ECS_inst.globalDataPools, dataPoolDescription.dataPoolTypeIndex_reg, 0);
// 	OCT_index dataIndex = eOCT_IDMap_getIndex(&iOCT_ECS_inst.globalDataMap, dataID);
//
// 	if (dataIndex >= dataPool->count) {
// 		return NULL;
// 	}
//
// 	void* dataLoc = eOCT_pool_access(dataPool, dataIndex, 0);
// 	return dataLoc;
// }

// void* eOCT_getGlobalDataField(eOCT_fieldRequest fieldRequest, OCT_ID dataID) {
// 	if (!fieldRequest.global_reg) {
// 		printf("Data pool %s is context local\n", fieldRequest.name);
// 		return NULL;
// 	}
// 	eOCT_pool* dataPool = (eOCT_pool*)eOCT_pool_access(&iOCT_ECS_inst.globalDataPools, fieldRequest.providerIndex_reg, 0);
// 	OCT_index dataIndex = eOCT_IDMap_getIndex(&iOCT_ECS_inst.globalDataMap, dataID);
//
// 	if (!dataPool || dataIndex >= dataPool->count) {
// 		return NULL;
// 	}
//
// 	void* dataLoc = eOCT_pool_access(dataPool, dataIndex, 0);
// 	return dataLoc;
// }

// OCT_handle iOCT_getContextHandle(const iOCT_entityContext* context) {
// 	return (OCT_handle) {
// 	.containerID = OCT_ID_NULL,
// 	.objectID = context->contextID
// 	};
// }
#pragma endregion

#pragma region static
// static OCT_index iOCT_ECS_addContextDataPool(eOCT_dataPoolDescription desc) {
// 	OCT_index index;
//
// 	size_t* strideDestination = (size_t*)eOCT_pool_addEntryOld(&iOCT_ECS_inst.dataPoolSizeAndOrderList, &index);
// 	*strideDestination = desc.stride;
//
// 	return index;
// }
// static OCT_index iOCT_ECS_addGlobalDataPool(eOCT_dataPoolDescription desc) {
// 	OCT_index index;
//
// 	eOCT_pool* poolDestination = eOCT_pool_addEntryOld(&iOCT_ECS_inst.globalDataPools, &index);
// 	*poolDestination = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, desc.stride);
//
// 	return index;
// }
#pragma endregion