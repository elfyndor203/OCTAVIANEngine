#include "systems_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>

#include "registry/registry_int.h"
#include "ECS/ECS_int.h"
#include "ECS/entityContexts/entityContexts_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"
#include "scheduler/scheduler_int.h"
#include "globals/globals_int.h"
#include "registry/registry_int.h"

OCT_ID eOCT_registry_registerSystem(eOCT_systemDescription systemDescription) {
	OCT_ID systemID = iOCT_registry_inst.systems_free.count + OCT_ID_SYSTEM_START;

	systemDescription.systemID_reg = systemID;
	eOCT_pool_addEntryNew(&iOCT_registry_inst.systems_free, &systemDescription, NULL);

	// eOCT_systemDescription** destination = (eOCT_systemDescription**)eOCT_pool_addEntryOld(&iOCT_registry_inst.systems, NULL);	// addEntry after so the ID starts at 3 instead of 3 + 1
	// *destination = systemDescription;
	// systemDescription->systemID_reg = systemID;
	printf("\n--------------------------------\n");
	printf("%02"PRIu64".--.--| System '%s':\n", systemDescription.systemID_reg, systemDescription.name);

	// COMPONENTS
	if (eOCT_pool_isEmpty(systemDescription.providedComponents)) {		// requests handed separately later, so registration ends
		printf("No provided components\n");
	}
	else {
		eOCT_componentDescription* componentArray = (eOCT_componentDescription*)systemDescription.providedComponents.array;	// register all components and all of their fields
		for (OCT_index componentCtr = 0; componentCtr < systemDescription.providedComponents.count; componentCtr++) {
			eOCT_componentDescription* component = &componentArray[componentCtr];
			printf("%02"PRIu64".%02zu.--| %2cComponent %zu: %-15s\n", systemID, component->componentTypeIndex_reg, ' ', component->componentTypeIndex_reg, component->name);

			iOCT_registry_registerComponent(component);
			iOCT_registry_registerFields(component->providedFields, systemID, component->componentTypeIndex_reg, false);
		}
	}

	printf("\n");
	// EVENTS
	if (eOCT_pool_isEmpty(systemDescription.providedEvents)) {
		printf("No provided events\n");
	}
	else {
		eOCT_eventDescription* eventArray = (eOCT_eventDescription*)systemDescription.providedEvents.array;
		for (OCT_index eventCtr = 0; eventCtr < systemDescription.providedEvents.count; eventCtr++) {
			eOCT_eventDescription* event = &eventArray[eventCtr];
			printf("%02"PRIu64".%02zu.--| %2cEvent %zu: %-15s\n", systemID, event->eventTypeIndex_reg, ' ', event->eventTypeIndex_reg, event->name);

			iOCT_registry_registerEvent(event);
			iOCT_registry_registerFields(event->providedFields, systemID, event->eventTypeIndex_reg, event->global);
		}
	}

	printf("\n");
	// DATA POOLS
	if (eOCT_pool_isEmpty(systemDescription.providedDataPools)) {
		printf("No additional provided data\n");
	}
	else {
		eOCT_dataPoolDescription* dataPoolArray = (eOCT_dataPoolDescription*)systemDescription.providedDataPools.array;
		for (OCT_index dataPoolCtr = 0; dataPoolCtr < systemDescription.providedDataPools.count; dataPoolCtr++) {
			eOCT_dataPoolDescription* dataPool = &dataPoolArray[dataPoolCtr];
			printf("%02"PRIu64".%02zu.--| %2cData Pool %zu: %-15s\n", systemID, dataPool->dataPoolTypeIndex_reg, ' ', dataPool->dataPoolTypeIndex_reg, dataPool->name);

			iOCT_registry_registerDataPool(dataPool);
			iOCT_registry_registerFields(dataPool->providedFields, systemID, dataPool->dataPoolTypeIndex_reg, dataPool->global);
		}
	}

	printf("\n");
	// SINGLES'
	if (eOCT_pool_isEmpty(systemDescription.providedSingles)) {
		printf("No provided singles\n");
	}
	else {
		eOCT_singleDescription* singlesArray = (eOCT_singleDescription*)systemDescription.providedSingles.array;
		for (OCT_index singleCtr = 0; singleCtr < systemDescription.providedSingles.count; singleCtr++) {
			eOCT_singleDescription* single = &singlesArray[singleCtr];
			printf("%02"PRIu64".%02zu.--| %2cSingle %zu: %-15s\n", systemID, single->singleTypeIndex_reg, ' ', single->singleTypeIndex_reg, single->name);
			if (single->providedField.offset != 0) {
				OCT_ERROR_LOG(OCT_EXIT_REGISTRATION_FAILED, "Singles must have offset 0");
				return OCT_ID_NULL;
			}

			iOCT_registry_registerSingle(single);
			iOCT_registry_registerField(&single->providedField, 0, systemID, single->singleTypeIndex_reg, single->global);
		}
	}

	eOCT_contextInitFx contextInitFx = systemDescription.contextInitFx;
	if (contextInitFx) {
		eOCT_pool_addEntryNew(&iOCT_registry_inst.contextInitFxs, &contextInitFx, NULL);
	}
	printf("--------------------------------\n\n");

	return systemID;
}