#pragma once
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"

#include "events/events_int.h"

#define iOCT_ROOT_ID 1
#define iOCT_COMPONENT_UNSET 0xFF
#define iOCT_ENTITY_ROOT_ID 1

/// <summary>
/// Opening an entityContext allows you to create entities that interact with each other. Each context manages its own pools and IDmap.
/// </summary>
struct iOCT_entityContext {
	OCT_ID contextID;

	eOCT_IDMap entityIDMap;		// uses a separate map + pool because entityIDs aren't stored in a known position within an entity
	eOCT_pool entities;
	eOCT_pool components;	// pool of eOCT_pools
	eOCT_pool dataPools;	// pool of eOCT_mappedPools
	eOCT_pool singles;		// pool of eOCT_dataUnions

	iOCT_eventManager events;
};

//iOCT_entityContext* iOCT_entityContext_get(OCT_ID entityContextID);
//eOCT_pool* iOCT_pool_get(iOCT_entityContext* context, OCT_ECSTypes componentType);
iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID);

eOCT_pool* iOCT_context_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex);
eOCT_pool* iOCT_getDataPool(iOCT_entityContext* context, OCT_index dataPoolTypeIndex);
void iOCT_entity_attachRootMeta(OCT_local entity);

eOCT_pool iOCT_entityContext_initComponentPools();
eOCT_pool iOCT_entityContext_initDataPools();
eOCT_pool iOCT_entityContext_initSingles();
OCT_local iOCT_entityContext_initRootEntity(iOCT_entityContext* context);
void iOCT_entityContext_initSystems(OCT_global contextHandle);

//void iOCT_entityContext_close(iOCT_entityContext* context);
//void* iOCT_getByID(iOCT_entityContext* context, OCT_ID ID, OCT_ECSTypes componentType);
//void iOCT_entityContext_update(iOCT_entityContext* context);