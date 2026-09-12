#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

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

eOCT_pool eOCT_generateDataPoolDescriptionPool(OCT_index total, eOCT_dataPoolDescription description1, ...);
eOCT_mappedPool* eOCT_dataPool_getGlobal(eOCT_dataPoolKey dataPoolKey);
eOCT_mappedPool* eOCT_dataPool_getLocal(eOCT_dataPoolKey dataPoolKey, OCT_global contextHandle);
