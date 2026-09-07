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

// eOCT_eventDescription iOCT_registry_findSourceEventDescription(eOCT_fieldRequest fieldRequest);
// void iOCT_registerAllSystems(); // defined in build_manifest.c
//void iOCT_allocateAllComponents();