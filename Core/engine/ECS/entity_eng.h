#pragma once
#include "types_eng.h"

#include "layout/types.h"

OCT_index eOCT_component_getIndex(OCT_local entity, eOCT_componentKey component);
OCT_local eOCT_entity_getHandle(OCT_local context, OCT_ID entityID);
OCT_global eOCT_entity_getContextHandle(OCT_local entity);
bool eOCT_component_isAttached(OCT_local entity, eOCT_componentKey component, bool* enabledOut);
bool eOCT_entity_isRoot(OCT_local entity);
//OCT_handle eOCT_entity_genContextHandle(OCT_handle entity);