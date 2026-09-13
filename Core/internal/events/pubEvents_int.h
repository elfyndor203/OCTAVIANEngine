#pragma once

#include "utilities/utilities_eng.h"

struct eOCT_eventListener {
    void* callbackFx;
    eOCT_pool queue;

    OCT_AorB callbackOrQueue;
};

struct eOCT_eventBroadcaster {
    bool global;

    eOCT_pool listeners;
};

