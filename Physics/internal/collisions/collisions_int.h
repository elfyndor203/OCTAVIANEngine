#pragma once
#include "physics2D/collisions.h"
#include "collider2D/collider2D.h"

#include "OCT_Core_eng.h"

#include "physicsSystem_int.h"

struct iOCT_collisionWatch {
    OCT_ID watchID;
    OCT_local colliderA;
    OCT_local colliderB;
    uint64_t pairValue;

    OCT_collision_states state;
    bool updated;
};

eOCT_DEFINE_DATAPOOL_LOCAL(iOCT_collisionWatch, iOCT_physicsSystem_inst.collisionWatchKey)

uint64_t iOCT_pairIDs(OCT_ID ID_x, OCT_ID ID_y);
