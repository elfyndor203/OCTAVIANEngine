#include "components_eng.h"

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

eOCT_pool eOCT_generateComponentDescriptionPool(OCT_index total, eOCT_componentDescription description1, ...) {
    if (total < 1) {
        OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no components are provided");
        return eOCT_POOL_EMPTY;
    }
    va_list args;
    va_start(args, description1);

    eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_componentDescription));
    bool end = false;
    OCT_index processed = 0;
    eOCT_componentDescription newRequest = description1;
    while (!end && processed < total) {
        if (strcmp(newRequest.name, eOCT_END_COMPONENTS.name) == 0) { // checks for END flag
            end = true;

            if (processed != total) {										// END flag should be after all requests are processed
                OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less components provided than expected");
                return pool;
            }
        } else {
            eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
            processed++;
            newRequest = va_arg(args, eOCT_componentDescription);
        }
    }
    eOCT_componentDescription expectedEnd = newRequest;	// most recent: either the END flag or error
    if (strcmp(expectedEnd.name, eOCT_END_COMPONENTS.name) != 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
        return pool;
    }

    va_end(args);
    return pool;
}

eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_pool* sourcePool = &((eOCT_pool*)context->components.array)[componentKey.componentTypeIndex];
    return sourcePool;
}