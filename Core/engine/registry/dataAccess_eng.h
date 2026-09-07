#pragma once
#include "../data/types_eng.h"

#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

// eOCT_pool* eOCT_field_getSourcePool(OCT_global contextHandle, eOCT_fieldTicket fieldTicket);
void* eOCT_field_read(eOCT_fieldTicket fieldTicket, OCT_index entryIndex, OCT_global contextHandle);

eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey);
eOCT_pool* eOCT_event_getPoolGlobal(eOCT_eventKey eventKey, eOCT_pool** callbackPoolOut);
eOCT_pool* eOCT_event_getPoolLocal(eOCT_eventKey eventKey, OCT_global contextHandle, eOCT_pool** callbackPoolOut);
eOCT_dataUnion* eOCT_single_getGlobal(eOCT_singleKey singleKey);
eOCT_dataUnion* eOCT_single_getLocal(eOCT_singleKey singleKey, OCT_global contextHandle);
eOCT_mappedPool* eOCT_dataPool_getGlobal(eOCT_dataPoolKey dataPoolKey);
eOCT_mappedPool* eOCT_dataPool_getLocal(eOCT_dataPoolKey dataPoolKey, OCT_global contextHandle);