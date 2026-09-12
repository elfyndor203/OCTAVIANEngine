#include "types_int.h"
#include "registry_int.h"
#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContext_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"
#include "scheduler/scheduler_int.h"
#include "globals/globals_int.h"
#include "registry/system/systems_eng.h"

#define GROUP_UNSET 0
#define GROUP_FAILED (-1)
#define GROUP_SUCCESS 1

static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut);
static eOCT_pool* iOCT_registry_findGlobalPool(eOCT_fieldDescription field);
static void iOCT_registry_distributeFields();
static void iOCT_registry_checkGroups();
static void iOCT_registry_buildGlobalKeys();

iOCT_registry iOCT_registry_inst = { 0 }; 

#pragma region init
void init_OCT_registry_init() {
	iOCT_registry_inst.systems_free = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_systemDescription));
	iOCT_registry_inst.fields = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_fieldDescription));

	iOCT_registry_inst.components = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_componentDescription));
	iOCT_registry_inst.globalDataPools = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataPoolDescription));
	iOCT_registry_inst.localDataPools = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataPoolDescription));
	iOCT_registry_inst.globalEvents = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_eventDescription));
	iOCT_registry_inst.localEvents = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_eventDescription));
	iOCT_registry_inst.globalSingles = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_singleDescription));
	iOCT_registry_inst.localSingles = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_singleDescription));
	iOCT_registry_inst.contextInitFxs = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_contextInitFx));
	iOCT_registry_inst.fieldGroups_free = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_fieldRequest));
	iOCT_registry_inst.success = true;

	printf("| Registry initialized\n");
}
void init_OCT_registry_distributeTickets() {
	iOCT_registry_distributeFields();
	iOCT_registry_checkGroups();
	iOCT_registry_buildGlobalKeys();
}
void init_OCT_registry_initAllSystems() {
	printf("INIT ALL SYSTEMS\n");
	eOCT_systemDescription system;
	eOCT_systemInitFx initFx;
	for (OCT_index systemCtr = 0; systemCtr < iOCT_registry_inst.systems_free.count; systemCtr++) {
		system = *(eOCT_systemDescription*)eOCT_pool_access(&iOCT_registry_inst.systems_free, systemCtr, 0);
		initFx = system.systemInitFx;
		if (initFx) {
			printf("Init system %s with INIT fx %p\n", system.name, initFx);
			initFx();
		}
		else {
			printf("System %s has no INIT fx\n", system.name);
		}
	}
}
void init_OCT_registry_summary() {
	eOCT_pool systemPool = iOCT_registry_inst.systems_free;
	eOCT_systemDescription* systemArray = (eOCT_systemDescription*)systemPool.array;
	eOCT_systemDescription system;
	int systemCtr = 0;

	eOCT_pool requestPool;
	eOCT_fieldRequest* requestArray;
	eOCT_fieldRequest request;
	int requestCtr = 0;

	printf("\n--------SUMMARY--------\n");
	printf("Systems: (%d - %d reserved)\n", 1, OCT_ID_SYSTEM_START - 1);
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		printf("%02zu.    | %s\n", system.systemID_reg, system.name);
	}

	printf("\nComponents:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		eOCT_pool componentPool = system.providedComponents;
		eOCT_componentDescription* componentArray = (eOCT_componentDescription*)componentPool.array;
		for (OCT_index componentCtr = 0; componentCtr < componentPool.count; componentCtr++) {
			eOCT_componentDescription component = componentArray[componentCtr];
			printf("%02zu.    | %s\n", component.componentTypeIndex_reg, component.name);
		}
	}
	// printf("ECS Component Count: %zu\n", iOCT_registry_inst.components.count);
	// printf("ECS Entity Size: %zu\n", iOCT_ECS_inst.entitySize);

	printf("\nData pools:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		eOCT_pool dataPools = system.providedDataPools;
		eOCT_dataPoolDescription* dataPoolArray = (eOCT_dataPoolDescription*)dataPools.array;
		for (OCT_index dataPoolCtr = 0; dataPoolCtr < dataPools.count; dataPoolCtr++) {
			eOCT_dataPoolDescription dataPool = dataPoolArray[dataPoolCtr];
			printf("%02zu.    | ", dataPool.dataPoolTypeIndex_reg);
			if (dataPool.global) {
				printf("[GLOBAL] ");
			} else {
				printf("[LOCAL] ");
			}
			printf("%s\n", dataPool.name);
		}
	}

	printf("\nEvents:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		eOCT_pool events = system.providedEvents;
		eOCT_eventDescription* eventArray = (eOCT_eventDescription*)events.array;
		for (OCT_index eventCtr = 0; eventCtr < events.count; eventCtr++) {
			eOCT_eventDescription event = eventArray[eventCtr];
			printf("%02zu.    | ", event.eventTypeIndex_reg);
			if (event.global) {
				printf("[GLOBAL] ");
			} else {
				printf("[LOCAL] ");
			}
			printf("%s\n", event.name);
		}
	}

	printf("\nSingles:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		eOCT_pool singles = system.providedSingles;
		eOCT_singleDescription* singleArray = (eOCT_singleDescription*)singles.array;
		for (OCT_index singleCtr = 0; singleCtr < singles.count; singleCtr++) {
			eOCT_singleDescription single = singleArray[singleCtr];
			printf("%02zu.    | ", single.singleTypeIndex_reg);
			if (single.global) {
				printf("[GLOBAL] ");
			} else {
				printf("[LOCAL] ");
			}
			printf("%s\n", single.name);
		}
	}

	printf("\nFields requested:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		requestPool = system.requestedFields;
		requestArray = (eOCT_fieldRequest*)requestPool.array;
		for (requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			request = requestArray[requestCtr];
			if (request.fulfilled_reg) {
				switch (request.providerType) {
				case eOCT_DATAPATTERN_COMPONENT:
					printf("C ");
					break;
				case eOCT_DATAPATTERN_DATAPOOL:
					printf("D ");
					break;
				case eOCT_DATAPATTERN_EVENT:
					printf("E ");
					break;
				case eOCT_DATAPATTERN_SINGLE:
					printf("S ");
					break;
				default:
					OCT_ERROR_LOG(OCT_EXIT_REGISTRATION_FAILED, "Unknown provider type");
				}
				printf("%02zu[%02zu]", request.providerIndex_reg, request.fieldOffset_reg);
			}
			else if (request.optional) {
				printf("XX[XX]");
			}
			else {
				printf("%7c| [FAIL]", ' ');
			}
			printf(" | %s\n", request.name);
		}
	}

	printf("\nRequest grouping status:\n");
	for (OCT_index groupCtr = 0; groupCtr < iOCT_registry_inst.fieldGroups_free.count; groupCtr++) {
		eOCT_fieldRequest* request = eOCT_pool_access(&iOCT_registry_inst.fieldGroups_free, groupCtr, 0);
		if (request->groupStatus_reg == GROUP_SUCCESS) {
			printf("Success | ");
			printf("Group: %s | ", request->groupName_opt);
		} else {
			printf("Failed: ");
		}
		printf("Field: %s\n", request->name);
	}

	printf("\nStatus: ");
	if (iOCT_registry_inst.success) {
		printf("Success\n");
	}
	else {
		printf("Failed\n");
		OCT_ERROR_LOG(OCT_EXIT_REGISTRATION_FAILED, "Check registration log to find failure");
	}
	printf("-----------------------\n\n");

}

void init_OCT_registry_cleanup() {
	eOCT_pool* systemPool = &iOCT_registry_inst.systems_free;
	for (OCT_index systemCtr = 0; systemCtr < systemPool->count; systemCtr++) {
		eOCT_systemDescription* system = eOCT_pool_access(systemPool, systemCtr, 0);
		eOCT_pool* componentsPool = &system->providedComponents;
		eOCT_pool* dataPoolsPool = &system->providedDataPools;
		eOCT_pool* eventsPool = &system->providedEvents;
		eOCT_pool* singlesPool = &system->providedSingles;
		eOCT_pool* requestsPool = &system->requestedFields;

		eOCT_pool* providedFieldsPool;
		for (OCT_index componentCtr = 0; componentCtr < componentsPool->count; componentCtr++) {
			eOCT_componentDescription* component = eOCT_pool_access(componentsPool, componentCtr, 0);
			providedFieldsPool = &component->providedFields;
			eOCT_pool_free(providedFieldsPool);
		}
		for (OCT_index dataPoolCtr = 0; dataPoolCtr < dataPoolsPool->count; dataPoolCtr++) {
			eOCT_dataPoolDescription* dataPool = eOCT_pool_access(dataPoolsPool, dataPoolCtr, 0);
			providedFieldsPool = &dataPool->providedFields;
			eOCT_pool_free(providedFieldsPool);
		}
		for (OCT_index eventsCtr = 0; eventsCtr < eventsPool->count; eventsCtr++) {
			eOCT_eventDescription* event = eOCT_pool_access(eventsPool, eventsCtr, 0);
			providedFieldsPool = &event->providedFields;
			eOCT_pool_free(providedFieldsPool);
		}
		// singles and fieldRequests don't contain pools

		eOCT_pool_free(componentsPool);
		eOCT_pool_free(dataPoolsPool);
		eOCT_pool_free(eventsPool);
		eOCT_pool_free(singlesPool);
		eOCT_pool_free(requestsPool);
	}
	eOCT_pool_free(systemPool);
	eOCT_pool_free(&iOCT_registry_inst.fieldGroups_free);
}
#pragma endregion



#pragma region dataPattern registration
void iOCT_registry_registerComponent(eOCT_componentDescription* componentDesc) {
	OCT_index componentIndex;
	eOCT_componentDescription* registryEntry = (eOCT_componentDescription*)eOCT_pool_addEntryNew(&iOCT_registry_inst.components, componentDesc, &componentIndex);	// store a stable copy in the registry
	componentDesc->componentTypeIndex_reg = componentIndex;	// inform the system of its component's index
	registryEntry->componentTypeIndex_reg = componentIndex;

	if (componentDesc->keyCacheLocation) {
		eOCT_componentKey key = {
			.name = componentDesc->name,
			.componentTypeIndex = componentDesc->componentTypeIndex_reg,
			.entityHandleValueOffset = componentDesc->entityHandleValueOffset
		};
		*componentDesc->keyCacheLocation = key;
	}
	else {
		OCT_ERROR_LOG(OCT_ERR_NOT_PROVIDED, "Ticket cache location not provided, component cannot be accessed without a ticket.");
	}
}
void iOCT_registry_registerDataPool(eOCT_dataPoolDescription* dataPoolDesc) {
	OCT_index dataPoolIndex;
	eOCT_dataPoolDescription* registryEntry;

	if (dataPoolDesc->global) {
		registryEntry = (eOCT_dataPoolDescription*)eOCT_pool_addEntryNew(&iOCT_registry_inst.globalDataPools, dataPoolDesc, &dataPoolIndex);
	} else {
		registryEntry = (eOCT_dataPoolDescription*)eOCT_pool_addEntryNew(&iOCT_registry_inst.localDataPools, dataPoolDesc, &dataPoolIndex);
	}
	dataPoolDesc->dataPoolTypeIndex_reg = dataPoolIndex;
	registryEntry->dataPoolTypeIndex_reg = dataPoolIndex;

	if (dataPoolDesc->keyCacheLocation) {
		eOCT_dataPoolKey key = {
			.name = dataPoolDesc->name,
			.dataPoolTypeIndex = dataPoolDesc->dataPoolTypeIndex_reg,
			.global = dataPoolDesc->global,
			.globalMappedPool = NULL	// set later
		};
		*dataPoolDesc->keyCacheLocation = key;
	}
}
void iOCT_registry_registerEvent(eOCT_eventDescription* eventDesc) {
	OCT_index eventIndex;
	eOCT_eventDescription* registryEntry;

	if (eventDesc->global) {
		registryEntry = (eOCT_eventDescription*)eOCT_pool_addEntryNew(&iOCT_registry_inst.globalEvents, eventDesc, &eventIndex);
	} else {
		registryEntry = (eOCT_eventDescription*)eOCT_pool_addEntryNew(&iOCT_registry_inst.localEvents, eventDesc, &eventIndex);
	}
	eventDesc->eventTypeIndex_reg = eventIndex;
	registryEntry->eventTypeIndex_reg = eventIndex;

	if (eventDesc->keyCacheLocation) {
		eOCT_eventKey key = {
			.name = eventDesc->name,
			.eventTypeIndex = eventDesc->eventTypeIndex_reg,
			.global = eventDesc->global,
			.globalEventPool = NULL		// set later
		};
		*eventDesc->keyCacheLocation = key;
	}
}
void iOCT_registry_registerSingle(eOCT_singleDescription* singleDesc) {
	// assert(singleDesc->global && "Context local singles not yet implemented");
	OCT_index singleIndex;
	eOCT_singleDescription* registryEntry;

	if (singleDesc->global) {
		registryEntry = (eOCT_singleDescription*)eOCT_pool_addEntryOld(&iOCT_registry_inst.globalSingles, &singleIndex);
	} else {
		registryEntry = (eOCT_singleDescription*)eOCT_pool_addEntryOld(&iOCT_registry_inst.localSingles, &singleIndex);
	}
	singleDesc->singleTypeIndex_reg = singleIndex;
	*registryEntry = *singleDesc;

	if (singleDesc->keyCacheLocation) {
		eOCT_singleKey key = {
			.name = singleDesc->name,
			.singleTypeIndex = singleDesc->singleTypeIndex_reg,
			.global = singleDesc->global,
			.globalPool = NULL
		};
		*singleDesc->keyCacheLocation = key;
	}
}
static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut) {
	eOCT_pool* fields = &iOCT_registry_inst.fields;
	//printf("Number of fields in registry: %d\n", fields->count);
	eOCT_fieldDescription* fieldArray = (eOCT_fieldDescription*)fields->array;
	eOCT_fieldDescription targetField;
	int fieldCtr = 0;

	for (fieldCtr = 0; fieldCtr < fields->count; fieldCtr++) {		// check every field in the registry
		targetField = fieldArray[fieldCtr];
		if (strcmp(targetField.name, fieldName) == 0 && targetField.providerType == targetField.providerType) {
			if (fieldOut) {
				*fieldOut = targetField;
			}
			return true;
		}
	}
	return false;
}
OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, OCT_index providerIndex, bool global) {
	if (eOCT_pool_isEmpty(providedFields)) {
		printf("%13c No public fields\n", ' ');
		return 0;
	}

	eOCT_fieldDescription* fieldArray = (eOCT_fieldDescription*)providedFields.array;
	eOCT_fieldDescription* field;
	eOCT_fieldDescription* fieldDestination;
	OCT_index fieldCtr = 0;
	for (fieldCtr = 0; fieldCtr < providedFields.count; fieldCtr++) {
		field = &fieldArray[fieldCtr];
		iOCT_registry_registerField(field, fieldCtr, systemID, providerIndex, global);
	}

	printf("%13c Fields: %zu\n", ' ', providedFields.count);
	return fieldCtr;
}
void iOCT_registry_registerField(eOCT_fieldDescription* field, OCT_index fieldNum, OCT_ID systemID, OCT_index providerIndex, bool global) {
	printf("%02"PRIu64".%02zu.%02zu| %4cField: %-15s | ", systemID, providerIndex, fieldNum, ' ', field->name);

	if (iOCT_registry_findField(field->name, NULL)) {	// check for duplicates
		printf("Failed: Field already exists\n");
		iOCT_registry_inst.success = false;
	}

	else {
		field->providerIndex_reg = providerIndex;
		field->global_reg = global;

		eOCT_fieldDescription* fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntryOld(&iOCT_registry_inst.fields, NULL);	// add field to the registry
		*fieldDestination = *field;
		printf("Success\n");
	}
}

// static bool iOCT_registry_validateComponent(eOCT_componentDescription component) {
// 	if (
// 		!component.keyCacheLocation ||
// 		!component.stride ||
// 		!component.entityHandleValueOffset ||
// 		(component.sort && component.sortValueOffset > ))
// }
// fills in global pools for global events
static void iOCT_registry_buildGlobalKeys() {
	eOCT_pool systemPool = iOCT_registry_inst.systems_free;
	eOCT_systemDescription* systemArray = (eOCT_systemDescription*)systemPool.array;

	for (int systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		eOCT_systemDescription system = systemArray[systemCtr];
		eOCT_pool* eventPool = &system.providedEvents;
		eOCT_pool* singlesPool = &system.providedSingles;
		eOCT_pool* dataPoolsPool = &system.providedDataPools;

		for (OCT_index eventCtr = 0; eventCtr < eventPool->count; eventCtr++) {
			eOCT_eventDescription* event = (eOCT_eventDescription*)eOCT_pool_access(eventPool, eventCtr, 0);
			eOCT_eventKey* key = event->keyCacheLocation;
			if (key->global) {
				key->globalEventPool = (eOCT_pool*)eOCT_pool_access(&iOCT_globals_inst.globalEvents.eventPools, key->eventTypeIndex, 0);
				key->globalCallbackPool = (eOCT_pool*)eOCT_pool_access(&iOCT_globals_inst.globalEvents.callbackPools, key->eventTypeIndex, 0);
			}
		}

		for (OCT_index singleCtr = 0; singleCtr < singlesPool->count; singleCtr++) {
			eOCT_singleDescription* single = (eOCT_singleDescription*)eOCT_pool_access(singlesPool, singleCtr, 0);
			eOCT_singleKey* key = single->keyCacheLocation;
			if (key->global) {
				key->globalPool = &iOCT_globals_inst.globalSingles;
			}
		}

		for (OCT_index dataPoolCtr = 0; dataPoolCtr < dataPoolsPool->count; dataPoolCtr++) {
			eOCT_dataPoolDescription* dataPool = (eOCT_dataPoolDescription*)eOCT_pool_access(dataPoolsPool, dataPoolCtr, 0);
			eOCT_dataPoolKey* key = dataPool->keyCacheLocation;
			if (key->global) {
				key->globalMappedPool = (eOCT_mappedPool*)eOCT_pool_access(&iOCT_globals_inst.globalDataPools, key->dataPoolTypeIndex, 0);
			}
		}

	}
}
static void iOCT_registry_distributeFields() {
	eOCT_pool systemPool = iOCT_registry_inst.systems_free;
	eOCT_systemDescription* systemArray = (eOCT_systemDescription*)systemPool.array;

	for (int systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		eOCT_systemDescription system = systemArray[systemCtr];
		eOCT_pool requestPool = system.requestedFields;

		if (eOCT_pool_isEmpty(requestPool)) {	// ensure pool has contents
			continue;
		}
		eOCT_fieldRequest* requestArray = (eOCT_fieldRequest*)requestPool.array;

		// for each request
		for (int requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			eOCT_fieldRequest* request = &requestArray[requestCtr];
			eOCT_fieldDescription match;

			eOCT_fieldTicket* ticket = request->ticketCache;
			if (iOCT_registry_findField(request->name, &match) && request->providerType == match.providerType) {	// if there is a match
				ticket->name = match.name;
				ticket->type = match.type;
				ticket->global = match.global_reg;
				ticket->providerType = match.providerType;
				ticket->offsetFromStruct = match.offset;
				ticket->providerTypeIndex = match.providerIndex_reg;
				ticket->globalPool = iOCT_registry_findGlobalPool(match);

				request->fulfilled_reg = true;
				request->global_reg = match.global_reg;
				request->fieldOffset_reg = match.offset;
				request->providerIndex_reg = match.providerIndex_reg;
				if (request->groupName_opt) {
					eOCT_pool_addEntryNew(&iOCT_registry_inst.fieldGroups_free, request, NULL);
				}
			}
			else {
				//printf("Failed to find existing field '%s'\n", request->name);
				request->fulfilled_reg = false;
				if (!request->optional) {
					iOCT_registry_inst.success = false;
				}
			}
		}
	}
}

static void iOCT_registry_checkGroups() {
	eOCT_pool* grouped = &iOCT_registry_inst.fieldGroups_free;

	for (OCT_index requestCtr = 0; requestCtr < grouped->count; requestCtr++) {
		eOCT_fieldRequest* request = eOCT_pool_access(grouped, requestCtr, 0);
		if (request->groupStatus_reg != GROUP_UNSET) {
			continue;
		}
		eOCT_dataPattern sourceType = request->providerType;
		OCT_index sourceIndex = request->providerIndex_reg;

		// actually check every group member
		int groupStatus = GROUP_SUCCESS;
		for (OCT_index searchCtr = requestCtr; searchCtr < grouped->count; searchCtr++) {
			eOCT_fieldRequest* groupMember = eOCT_pool_access(grouped, searchCtr, 0);
			if (groupMember->groupStatus_reg ||
				(strcmp(request->groupName_opt, groupMember->groupName_opt) != 0)) {
				// printf("%s %s are different groups or already grouped\n", request->groupName_opt, groupMember.groupName_opt);
				continue;
			}
			if (sourceType != groupMember->providerType || sourceIndex != groupMember->providerIndex_reg) {
				groupStatus = GROUP_FAILED;
			}
		}

		// mark all as grouped if success
		if (groupStatus == GROUP_FAILED) {
			iOCT_registry_inst.success = false;
		}
		for (OCT_index searchCtr = 0; searchCtr < grouped->count; searchCtr++) {
			eOCT_fieldRequest* groupMember = eOCT_pool_access(grouped, searchCtr, 0);
			if (strcmp(request->groupName_opt, groupMember->groupName_opt) != 0) {
				continue;
			}
			groupMember->groupStatus_reg = groupStatus;
		}
		request->groupStatus_reg = groupStatus;
	}
}

static eOCT_pool* iOCT_registry_findGlobalPool(eOCT_fieldDescription field) {
	if (!field.global_reg) {
		return NULL;
	}

	eOCT_pool* pool;
	switch (field.providerType) {
	case eOCT_DATAPATTERN_COMPONENT:
		return NULL;
	case eOCT_DATAPATTERN_DATAPOOL:
		pool = &((eOCT_mappedPool*)iOCT_globals_inst.globalDataPools.array)[field.providerIndex_reg].pool;
		return NULL;
	case eOCT_DATAPATTERN_EVENT:
		pool = &((eOCT_pool*)iOCT_globals_inst.globalEvents.eventPools.array)[field.providerIndex_reg];
		break;
	case eOCT_DATAPATTERN_SINGLE:
		pool = &iOCT_globals_inst.globalSingles;
		break;
	default:
		return NULL;
	}

	return pool;
}

#pragma endregion

