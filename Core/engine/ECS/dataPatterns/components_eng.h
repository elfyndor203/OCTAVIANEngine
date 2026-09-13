#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

#define eOCT_END_COMPONENTS ((eOCT_componentDescription){.name = "COMPONENT_DESCRIPTION_END"})

#define eOCT_DEFINE_COMPONENT(componentName, componentKey)\
    static inline componentName* componentName##_get(OCT_local entityHandle) { \
        return (componentName*)eOCT_component_get(entityHandle, componentKey); \
    } \
    \
    static inline eOCT_pool* componentName##_getPool(OCT_global contextHandle) { \
        return eOCT_component_getPool(contextHandle, componentKey); \
    } \
    \
    static inline componentName* componentName##_attach(OCT_local entity, componentName* source, OCT_index* outIndex ) { \
        return eOCT_component_attach(entity, componentKey, source, outIndex); \
    } \


#define eOCT_DEFINE_COMPONENT_FIELD_ACCESSOR(fieldName, fieldType, fieldTicket)\
    static inline fieldType* fieldName##_getField(OCT_local entityHandle) { \
    return (fieldType*)eOCT_component_getField(entityHandle, fieldTicket); \
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
void* eOCT_component_get(OCT_local entity, eOCT_componentKey component);
void* eOCT_component_attach(OCT_local entity, eOCT_componentKey componentKey, void* source, OCT_index* outIndex);
void* eOCT_component_getFieldByToken(eOCT_contextToken contextToken, OCT_local entity, eOCT_fieldTicket field);
void* eOCT_component_getField(OCT_local entity, eOCT_fieldTicket field);