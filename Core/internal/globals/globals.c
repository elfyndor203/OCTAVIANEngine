#include "globals_int.h"

#include <assert.h>

#include "events/events_int.h"
#include "layout/systems.h"
#include "registry/registry_int.h"
#include "../../engine/ECS/dataPatterns/dataPools_eng.h"

iOCT_globals iOCT_globals_inst = { 0 };

void init_OCT_globals_init() {
    iOCT_globals_inst.globalSingles = eOCT_pool_open(OCT_ID_GLOBALS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataUnion));
    iOCT_globals_inst.globalDataPools = eOCT_pool_open(OCT_ID_GLOBALS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_mappedPool));
}

void init_OCT_globals_build() {
    iOCT_globals_inst.globalEvents = iOCT_eventManager_open(OCT_ID_GLOBALS);
    for (OCT_index singleCtr = 0; singleCtr < iOCT_registry_inst.globalSingles.count; singleCtr++) {
        // eOCT_pool_addEntryOld(&iOCT_globals_inst.globalSingles, NULL);
        eOCT_pool_addEntryNew(&iOCT_globals_inst.globalSingles, NULL, NULL);
    }
    for (OCT_index dataPoolCtr = 0; dataPoolCtr < iOCT_registry_inst.globalDataPools.count; dataPoolCtr++) {
        eOCT_dataPoolDescription* dataPool = (eOCT_dataPoolDescription*)eOCT_pool_access(&iOCT_registry_inst.globalDataPools, dataPoolCtr, 0);

        eOCT_mappedPool newMPool = eOCT_mappedPool_open(OCT_ID_GLOBALS, eOCT_POOL_CAPACITY_DEFAULT, dataPool->stride, dataPool->elementIDValueOffset);
        eOCT_pool_addEntryNew(&iOCT_globals_inst.globalDataPools, &newMPool, NULL);
    }
}

void eOCT_globals_update() {
    iOCT_eventManager_clear(&iOCT_globals_inst.globalEvents);
}