#pragma once
#include "types_eng.h"

#include "layout/types.h"
#include "registry/registry_eng.h"

#define eOCT_DEFINE_COMPONENT_ACCESSOR(componentName, keyHolder, keyHolderMember)\
    static inline componentName* componentName##_get(OCT_local entityHandle) { \
        return (componentName*)eOCT_entity_getComponent(entityHandle, keyHolder.keyHolderMember); \
    }

#define eOCT_DEFINE_COMPONENT_FIELD_ACCESSOR(fieldName, fieldType, ticketHolder, ticketHolderMember)\
    static inline fieldType* fieldName##_get(OCT_local entityHandle) { \
        return (fieldType*)eOCT_entity_getFieldOnce(entityHandle, ticketHolder.ticketHolderMember); \
    }

void* eOCT_entity_attachComponent(OCT_local entity, eOCT_componentKey componentKey, void* source, OCT_index* outIndex);
// void* eOCT_entity_getComponent(eOCT_contextToken contextToken, OCT_local entity, eOCT_componentKey component);
void* eOCT_entity_getComponent(OCT_local entity, eOCT_componentKey component);
void* eOCT_entity_getField(eOCT_contextToken contextToken, OCT_local entity, eOCT_fieldTicket field);
void* eOCT_entity_getFieldOnce(OCT_local entity, eOCT_fieldTicket field);
OCT_index eOCT_entity_getComponentIndex(OCT_local entity, eOCT_componentKey component);
OCT_local eOCT_entity_getHandle(OCT_local context, OCT_ID entityID);
OCT_global eOCT_entity_getContextHandle(OCT_local entity);
bool eOCT_entity_hasComponent(OCT_local entity, eOCT_componentKey component, bool* enabledOut);
bool eOCT_entity_isRoot(OCT_local entity);
//OCT_handle eOCT_entity_genContextHandle(OCT_handle entity);