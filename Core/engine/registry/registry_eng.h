#pragma once
#include "types_eng.h"
#include "ECS/types_eng.h"

#include "utilities/utilities_eng.h"
#include "dataTypes_eng.h"
#include "fields/fields_eng.h"

#define eOCT_END_FIELDS ((eOCT_fieldDescription){.name = "FIELD_DESCRIPTION_END"})
#define eOCT_END_COMPONENTS ((eOCT_componentDescription){.name = "COMPONENT_DESCRIPTION_END"})
#define eOCT_END_DATAPOOLS ((eOCT_dataPoolDescription){.name = "DATAPOOL_DESCRIPTION_END"})
#define eOCT_END_EVENTS ((eOCT_eventDescription){.name = "EVENTS"})
#define eOCT_END_SINGLES ((eOCT_singleDescription){.name = "SINGLE_DESCRIPTION_END"})
#define eOCT_END_REQUESTS ((eOCT_fieldRequest){.name = "FIELD_REQUEST_END"})



/*!
 * Registers each system's components, data pools, and events into the registry. For each component, data pool, and event, registers each of its fields. Engine init fails if any duplicate fields are found.
 * @param systemDescription
 */
