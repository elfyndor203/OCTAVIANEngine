#pragma once
#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

#define eOCT_END_DATAPOOLS ((eOCT_dataPoolDescription){.name = "DATAPOOL_DESCRIPTION_END"})

#define eOCT_DEFINE_DATAPOOL_GLOBAL(dataName, dataPoolKey) \
    static inline eOCT_mappedPool* dataName##_getPool() { \
        return eOCT_dataPool_getGlobal(dataPoolKey); \
    } \
    static inline dataName* dataName##_get(OCT_ID entryID) { \
        eOCT_mappedPool* mappedPool = eOCT_dataPool_getGlobal(dataPoolKey); \
        return eOCT_mappedPool_getByID(mappedPool, entryID); \
    } \
    static inline dataName* dataName##_new(dataName* source, OCT_ID* outID, OCT_index* outIndex) { \
        eOCT_mappedPool* mappedPool = eOCT_dataPool_getGlobal(dataPoolKey); \
        return eOCT_mappedPool_addEntry(mappedPool, source, outID, outIndex); \
    }

#define eOCT_DEFINE_DATAPOOL_LOCAL(dataName, dataPoolKey) \
    static inline eOCT_mappedPool* dataName##_getPool(OCT_global contextHandle) { \
        return eOCT_dataPool_getLocal(dataPoolKey, contextHandle); \
    } \
    static inline dataName* dataName##_get(OCT_global contextHandle, OCT_ID entryID) { \
        eOCT_mappedPool* mappedPool = eOCT_dataPool_getLocal(dataPoolKey, contextHandle); \
        return eOCT_mappedPool_getByID(mappedPool, entryID); \
    } \
    static inline dataName* dataName##_new(OCT_global contextHandle, dataName* source, OCT_ID* outID, OCT_index* outIndex) { \
        eOCT_mappedPool* mappedPool = eOCT_dataPool_getLocal(dataPoolKey, contextHandle); \
        return eOCT_mappedPool_addEntry(mappedPool, source, outID, outIndex); \
    }

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
