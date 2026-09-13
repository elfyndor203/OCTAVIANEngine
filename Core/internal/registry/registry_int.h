#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

struct iOCT_registry {
	eOCT_pool systems_free;	// systemDesc list, will be cleaned up at the end of init
	eOCT_pool fieldGroups_free;

	eOCT_pool components; // stable copies
	eOCT_pool globalDataPools;
	eOCT_pool localDataPools;
	eOCT_pool globalEvents;
	eOCT_pool localEvents;
	eOCT_pool globalSingles;
	eOCT_pool localSingles;
	eOCT_pool contextInitFxs;

	eOCT_pool fields;	  // field copies

	bool success;
};

extern iOCT_registry iOCT_registry_inst;

void iOCT_registry_registerComponent(eOCT_componentDescription* componentDesc);
void iOCT_registry_registerEvent(eOCT_eventDescription* eventDesc);
void iOCT_registry_registerDataPool(eOCT_dataPoolDescription* dataPoolDesc);
void iOCT_registry_registerSingle(eOCT_singleDescription* singleDesc);

void iOCT_registry_registerField(eOCT_fieldDescription* field, OCT_index fieldNum, OCT_ID systemID, OCT_index providerIndex, bool global);
OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, OCT_index providerIndex, bool global);
