#include "allocations_int.h"
#include "layout/systems.h"

#include <stdio.h>
#include <inttypes.h>

iOCT_memoryManager iOCT_memoryManager_inst = {0};

void init_OCT_memoryManager_init() {
    iOCT_memoryManager_inst.allocationRefs = eOCT_pool_open(OCT_ID_MEMORY_MANAGER, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_allocationRef));
    iOCT_memoryManager_inst.allocationRefs.allocationRefIndex = 0;
    iOCT_memoryManager_logAlloc(OCT_ID_MEMORY_MANAGER, iOCT_ALLOCATION_POOL, sizeof(iOCT_allocationRef));
}

OCT_index iOCT_memoryManager_logAlloc(OCT_ID ownerID, iOCT_allocationTypes type, size_t elementSize) {
    iOCT_allocationRef newRef = {
        .ownerSystemID = ownerID,
        .type = type,
        .elementSize = elementSize,
        .freed = false
    };
    OCT_index index;
    eOCT_pool_addEntryNew(&iOCT_memoryManager_inst.allocationRefs, &newRef, &index);
    return index;
}

void iOCT_memoryManager_logFree(OCT_index allocationRefIndex) {
    iOCT_allocationRef* ref = eOCT_pool_access(&iOCT_memoryManager_inst.allocationRefs, allocationRefIndex, 0);
    ref->freed = true;
}

void OCT_memory_check() {
    bool unfreed = false;

    OCT_index freedCtr = 0;
    OCT_index unfreedCtr = 0;
    for (OCT_index refCtr = 0; refCtr < iOCT_memoryManager_inst.allocationRefs.count; refCtr++) {
        iOCT_allocationRef ref = *(iOCT_allocationRef*)eOCT_pool_access(&iOCT_memoryManager_inst.allocationRefs, refCtr, 0);
        if (ref.type == iOCT_ALLOCATION_POOL) {
            printf("Pool: \n");
        } else if (ref.type == iOCT_ALLOCATION_MAP) {
            printf("Map: \n");
        }

        printf("  Source ID: %"PRIu64, ref.ownerSystemID);
        if (ref.ownerSystemID < OCT_ID_SYSTEM_START) {
            printf(" (CORE)");
        }
        printf("\n");
        printf("  Element size: %zu\n", ref.elementSize);
        printf("  Freed: ");
        if (ref.freed) {
            printf("Yes\n");
            freedCtr++;
        } else {
            printf("No\n");
            unfreedCtr++;
            unfreed = true;
        }
    }

    printf("Total pools and maps: %zu\n", iOCT_memoryManager_inst.allocationRefs.count);
    printf("Freed: %zu\n", freedCtr);
    printf("Unfreed: %zu\n", unfreedCtr);
    if (unfreed) {
        printf("Unfreed allocations exist\n");
    }
}