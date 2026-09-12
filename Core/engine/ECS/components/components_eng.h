#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

#define eOCT_DEFINE_COMPONENT(componentName, keyHolder)\
    static inline componentName* componentName##_get(OCT_local entityHandle) { \
    return (componentName*)eOCT_entity_getComponent(entityHandle, keyHolder); \
    } \
    static inline eOCT_pool* componentName##_getPool(OCT_global contextHandle) { \
    return eOCT_component_getPool(contextHandle, keyHolder); \
    }

#define eOCT_DEFINE_COMPONENT_FIELD_ACCESSOR(fieldName, fieldType, ticketHolder, ticketHolderMember)\
    static inline fieldType* fieldName##_getField(OCT_local entityHandle) { \
    return (fieldType*)eOCT_entity_getField(entityHandle, ticketHolder.ticketHolderMember); \
    }

struct eOCT_componentDescription {
    const char* name;
    size_t stride;
    eOCT_pool providedFields;
    eOCT_componentKey* keyCacheLocation;
    eOCT_rootAttachmentFx rootAttachmentFx;
    OCT_index entityHandleValueOffset;

    bool sort;
    OCT_index sortValueOffset;

    OCT_index componentTypeIndex_reg; // where the component is located in the ECS
};
struct eOCT_componentKey {
    const char* name;

    OCT_index componentTypeIndex;
    OCT_index entityHandleValueOffset;
};

eOCT_pool eOCT_generateComponentDescriptionPool(OCT_index total, eOCT_componentDescription description1, ...);
eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey);