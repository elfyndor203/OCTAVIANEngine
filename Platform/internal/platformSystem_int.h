#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_platformSystem {
    OCT_ID systemID;
    eOCT_singleKey timeKey;
    eOCT_singleKey deltaTimeKey;
    double previousFrameTime;
};

extern iOCT_platformSystem iOCT_platformSystem_inst;

eOCT_DEFINE_SINGLE_GLOBAL(iOCT_time, double, iOCT_platformSystem_inst.timeKey)
eOCT_DEFINE_SINGLE_GLOBAL(iOCT_deltaTime, double, iOCT_platformSystem_inst.deltaTimeKey)

void iOCT_platformSystem_init();