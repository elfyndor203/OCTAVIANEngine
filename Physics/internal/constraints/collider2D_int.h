#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <box2d/box2d.h>

#include "physicsSystem_int.h"

struct iOCT_collider2D {
    OCT_ID colliderID;

    OCT_shapeType shape;
    OCT_vec2 origin;
    OCT_vec2 dimensions;
    float rotation;

    b2ShapeId b2ShapeID;
};

eOCT_DEFINE_DATAPOOL_LOCAL(iOCT_collider2D, iOCT_physicsSystem_inst.collider2DKey)

