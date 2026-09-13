#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <box2d/box2d.h>

#include "physicsSystem_int.h"

struct iOCT_distance2D {
    OCT_ID distanceID;

    b2JointId jointID;
};

eOCT_DEFINE_DATAPOOL_LOCAL(iOCT_distance2D, iOCT_physicsSystem_inst.distance2DKey)
