#pragma once
#include "ECS/entityContexts/entityContexts_int.h"

void* iOCT_component_get(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex);
/*!
 * Returns the slot in the entity where the given component's index is stored.
 * @param context
 * @param entityIndex
 * @param component
 * @return componentSlotPtr
 */
OCT_index* iOCT_component_getEntitySlot(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component);