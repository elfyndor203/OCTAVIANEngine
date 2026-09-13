#pragma once

#include "layout/types.h"
#include "registry/registry_eng.h"

/*!
 * A contextToken is needed to access local fields owned by other systems.
 */
typedef struct eOCT_contextToken eOCT_contextToken;
typedef void (*eOCT_rootAttachmentFx)(OCT_local rootEntity);
typedef void (*eOCT_eventCallbackFx)(OCT_index eventIndex);


/*!
 * Describes one component provided by the system. Components are 1-1 with entities are always local.
 * Leave all _reg fields blank.
 * Only include fields that are publicly visible. Component structs may have unincluded, private fields.
 */
typedef struct eOCT_componentDescription eOCT_componentDescription;
typedef struct eOCT_dataPoolDescription eOCT_dataPoolDescription;
typedef struct eOCT_eventDescription eOCT_eventDescription;
typedef struct eOCT_singleDescription eOCT_singleDescription;

typedef struct eOCT_componentKey eOCT_componentKey;
typedef struct eOCT_dataPoolKey eOCT_dataPoolKey;
typedef struct eOCT_eventKey eOCT_eventKey;
typedef struct eOCT_singleKey eOCT_singleKey;