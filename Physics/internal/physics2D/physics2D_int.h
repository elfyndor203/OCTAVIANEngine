#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdbool.h>
#include <box2d/box2d.h>

#include "physicsSystem_int.h"

struct iOCT_physics2D_oct {
    OCT_local entityHandle;

    float mass;
    float inertia;
    float gravityStrength;

    OCT_vec2 velocity;
    float angVelocity;

    OCT_vec2 f_frame;
    OCT_vec2 f_const;

    OCT_vec2 prevPos;
    bool fixed;
};

struct iOCT_physics2D_b2 {
    OCT_local entityHandle;

    b2BodyId b2dBodyID;

    float maxSpeedX;
    float maxSpeedY;
};

eOCT_DEFINE_COMPONENT_ACCESSOR(iOCT_physics2D_b2, iOCT_physicsSystem_inst, physics2DKey)
void iOCT_physics2D_integrateEulerOld(iOCT_physics2D_oct* physics2D, OCT_vec2* position, float* rotation, float dt);
void iOCT_physics2D_conserveL(iOCT_physics2D_oct* center, iOCT_physics2D_oct* target, float originalDistance, float newDistance);