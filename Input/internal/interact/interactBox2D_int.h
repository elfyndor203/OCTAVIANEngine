#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

struct iOCT_interactBox2D {
    OCT_local entity;

    OCT_shape2 shape;
    OCT_index layer;

    float interactTime;
};

eOCT_DEFINE_COMPONENT(iOCT_interactBox2D, iOCT_inputSystem_inst.interactBoxKey)