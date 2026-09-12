#pragma once
#include "registry/types_eng.h"

#include "ECS/types_eng.h"
#include "utilities/utilities_eng.h"

struct eOCT_systemDescription {
    const char* name;
    eOCT_pool providedComponents;
    eOCT_pool providedDataPools;
    eOCT_pool providedEvents;
    eOCT_pool providedSingles;
    eOCT_pool requestedFields;

    eOCT_contextInitFx contextInitFx;
    eOCT_systemInitFx systemInitFx;
    // eOCT_systemUpdateFx updateFx;

    OCT_ID systemID_reg; // provided by the registry
};

OCT_ID eOCT_registry_registerSystem(eOCT_systemDescription systemDescription);