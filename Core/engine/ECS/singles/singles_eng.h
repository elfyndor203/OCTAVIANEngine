#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

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

eOCT_pool eOCT_generateSingleDescriptionPool(OCT_index total, eOCT_singleDescription description1, ...);
eOCT_dataUnion* eOCT_single_getGlobal(eOCT_singleKey singleKey);
eOCT_dataUnion* eOCT_single_getLocal(eOCT_singleKey singleKey, OCT_global contextHandle);