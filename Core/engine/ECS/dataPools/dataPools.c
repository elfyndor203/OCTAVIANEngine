#include "dataPools_eng.h"

#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContext_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"

eOCT_pool eOCT_generateDataPoolDescriptionPool(OCT_index total, eOCT_dataPoolDescription description1, ...) {
    if (total < 1) {
        OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no dataPools are provided");
        return eOCT_POOL_EMPTY;
    }
    va_list args;
    va_start(args, description1);

    eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_dataPoolDescription));
    bool end = false;
    OCT_index processed = 0;
    eOCT_dataPoolDescription newRequest = description1;
    while (!end && processed < total) {
        if (strcmp(newRequest.name, eOCT_END_DATAPOOLS.name) == 0) { // checks for END flag
            end = true;

            if (processed != total) {										// END flag should be after all requests are processed
                OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less dataPools provided than expected");
                return pool;
            }
        } else {
            eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
            processed++;
            newRequest = va_arg(args, eOCT_dataPoolDescription);
        }
    }
    eOCT_dataPoolDescription expectedEnd = newRequest;	// most recent: either the END flag or error
    if (strcmp(expectedEnd.name, eOCT_END_DATAPOOLS.name) != 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
        return pool;
    }

    va_end(args);
    return pool;
}

eOCT_mappedPool* eOCT_dataPool_getGlobal(eOCT_dataPoolKey dataPoolKey) {
    if (!dataPoolKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided dataPool is local. Accessed globally.");
    }
    return dataPoolKey.globalMappedPool;
}
eOCT_mappedPool* eOCT_dataPool_getLocal(eOCT_dataPoolKey dataPoolKey, OCT_global contextHandle) {
    if (dataPoolKey.global) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Provided dataPool is global. Accessed locally.");
    }
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
    eOCT_mappedPool* mPool = eOCT_pool_access(&context->dataPools, dataPoolKey.dataPoolTypeIndex, 0);
    return mPool;
}