#include "systemDescription_platform.h"

#include "OCT_Core_eng.h"

#include "platformSystem_int.h"

void system_register_PLATFORM() {
    eOCT_fieldDescription time = {
        .name = "runningTime",
        .type = eOCT_TYPE_DOUBLE64,
        .providerType = eOCT_DATAPATTERN_SINGLE,
        .offset = 0
    };
    eOCT_singleDescription timeSingle = {
        .name = "time",
        .providedField = time,
        .global = true,
        .keyCacheLocation = &iOCT_platformSystem_inst.timeTicket
    };
    eOCT_fieldDescription deltaTime = {
        .name = "deltaFrameTime",
        .type = eOCT_TYPE_DOUBLE64,
        .providerType = eOCT_DATAPATTERN_SINGLE,
        .offset = 0
    };
    eOCT_singleDescription deltaTimeSingle = {
        .name = "deltaTime",
        .providedField = deltaTime,
        .global = true,
        .keyCacheLocation = &iOCT_platformSystem_inst.deltaTimeTicket
    };

    eOCT_systemDescription platformSystem = {
        .name = "Platform",
        .providedComponents = eOCT_POOL_EMPTY,
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_generateSingleDescriptionPool(2, timeSingle, deltaTimeSingle, eOCT_END_SINGLES),
        .systemInitFx = iOCT_platformSystem_init
    };

    iOCT_platformSystem_inst.systemID = eOCT_registry_registerSystem(platformSystem);
}
