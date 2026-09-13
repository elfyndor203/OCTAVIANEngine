#pragma once
#include "collider2D/collider2D.h"
#include "constraints/types_int.h"
#include "physics2D/collisions.h"

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

    bool watchCollision;
    OCT_collider2D_collisionCallback callback;
};

eOCT_DEFINE_DATAPOOL_LOCAL(iOCT_collider2D, iOCT_physicsSystem_inst.collider2DKey)

void iOCT_collider2D_callWatches(OCT_global context);

