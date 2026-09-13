#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

#define eOCT_DEFINE_SINGLE_GLOBAL(singleName, typeName, singleKey) \
    static inline typeName* singleName##_get() { \
        return (typeName*)eOCT_single_getGlobal(singleKey); \
    } \

#define eOCT_DEFINE_SINGLE_LOCAL(singleName, typeName, singleKey) \
    static inline typeName* singleName##_get(OCT_global contextHandle) { \
        return (typeName*)eOCT_single_getLocal(singleKey, contextHandle); \
    } \

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