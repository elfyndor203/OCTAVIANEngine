#pragma once

#include "OCT_Core.h"

typedef enum OCT_collision_states {
    OCT_COLLISION_NONE,
    OCT_COLLISION_COLLIDED,
    OCT_COLLISION_TOUCHING,
    OCT_COLLISION_EXITED
} OCT_collision_states;

typedef void (*OCT_collider2D_collisionCallback)(OCT_local watchedCollider, OCT_local otherCollider, OCT_collision_states state, void* userData);

OCT_local OCT_collider2D_new(OCT_local entity, OCT_shapeType shape, OCT_vec2 dimensions, OCT_vec2 origin, float radians, float density);
void OCT_collider2D_watch(OCT_local colliderHandle, OCT_collider2D_collisionCallback callback, void* userData);