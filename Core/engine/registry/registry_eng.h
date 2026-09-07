#pragma once
#include "types_eng.h"
#include "ECS/types_eng.h"

#include "utilities/utilities_eng.h"
#include "dataTypes_eng.h"
#include "dataPatterns_eng.h"
#include "fields_eng.h"

#define eOCT_END_FIELDS ((eOCT_fieldDescription){.name = "FIELD_DESCRIPTION_END"})
#define eOCT_END_COMPONENTS ((eOCT_componentDescription){.name = "COMPONENT_DESCRIPTION_END"})
#define eOCT_END_DATAPOOLS ((eOCT_dataPoolDescription){.name = "DATAPOOL_DESCRIPTION_END"})
#define eOCT_END_EVENTS ((eOCT_eventDescription){.name = "EVENTS"})
#define eOCT_END_SINGLES ((eOCT_singleDescription){.name = "SINGLE_DESCRIPTION_END"})
#define eOCT_END_REQUESTS ((eOCT_fieldRequest){.name = "FIELD_REQUEST_END"})

struct eOCT_systemDescription {
	const char* name;
	eOCT_pool providedComponents;
	eOCT_pool providedDataPools;
	eOCT_pool providedEvents;
	eOCT_pool providedSingles;
	eOCT_pool requestedFields;

	eOCT_contextInitFx contextInitFx;
	eOCT_systemInitFx systemInitFx;
	// eOCT_systemUpdateFx updateFx;

	OCT_ID systemID_reg; // provided by the registry
};

/*!
 * Registers each system's components, data pools, and events into the registry. For each component, data pool, and event, registers each of its fields. Engine init fails if any duplicate fields are found.
 * @param systemDescription
 */
OCT_ID eOCT_registry_registerSystem(eOCT_systemDescription systemDescription);
eOCT_pool eOCT_generateFieldDescriptionPool(OCT_index total, eOCT_fieldDescription description1, ...);
eOCT_pool eOCT_generateComponentDescriptionPool(OCT_index total, eOCT_componentDescription description1, ...);
eOCT_pool eOCT_generateDataPoolDescriptionPool(OCT_index total, eOCT_dataPoolDescription description1, ...);
eOCT_pool eOCT_generateEventDescriptionPool(OCT_index total, eOCT_eventDescription description1, ...);
eOCT_pool eOCT_generateSingleDescriptionPool(OCT_index total, eOCT_singleDescription description1, ...);
eOCT_pool eOCT_generateFieldRequestPool(OCT_index total, eOCT_fieldRequest request1, ...);