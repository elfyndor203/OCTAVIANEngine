#pragma once
#include "types_eng.h"

#include "dataTypes_eng.h"
#include "layout/types.h"
#include "utilities/utilities_eng.h"
#include "ECS/types_eng.h"
#include "fields_eng.h"

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

struct eOCT_dataPoolDescription {
    const char* name;
    size_t stride;
    eOCT_pool providedFields;
    size_t elementIDValueOffset;
    eOCT_dataPoolKey* keyCacheLocation;
    bool global;

    bool sort;
    OCT_index sortValueOffset;

    OCT_index dataPoolTypeIndex_reg;
};
struct eOCT_dataPoolKey {
    const char* name;

    OCT_index dataPoolTypeIndex;
    bool global;
    eOCT_mappedPool* globalMappedPool;
};

struct eOCT_eventDescription { // for cross module communication, but what about for the user __NOTE__
    const char* name;
    size_t stride;
    eOCT_pool providedFields;
    eOCT_eventKey* keyCacheLocation;
    bool global;

    OCT_index eventTypeIndex_reg;
};
struct eOCT_eventKey {
    const char* name;

    OCT_index eventTypeIndex;
    bool global;
    eOCT_pool* globalEventPool;
    eOCT_pool* globalCallbackPool;
};

struct eOCT_singleDescription {
    const char* name;
    eOCT_fieldDescription providedField;
    eOCT_singleKey* keyCacheLocation;
    bool global;

    OCT_index singleTypeIndex_reg;
};
struct eOCT_singleKey {
    const char* name;

    OCT_index singleTypeIndex;
    bool global;
    eOCT_pool* globalPool;
};