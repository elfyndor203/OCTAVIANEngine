#pragma once
#include "OCT_Core.h"

// OCT_local OCT_rope2D_new_OLD(OCT_local entityA, OCT_local entityB, float length);
// void OCT_rope2D_length_OLD(OCT_local rope2D, float newLength);
// bool OCT_rope2D_disable_OLD(OCT_local rope2D);
// bool OCT_rope2D_enable_OLD(OCT_local rope2D);
//
// OCT_local OCT_hitbox2D_new_OLD(OCT_local entity, OCT_vec2 dimensions, OCT_vec2 position, float rotation);

OCT_local OCT_distance2D_constrain(OCT_local entityA, OCT_local entityB, OCT_vec2 anchorA, OCT_vec2 anchorB, float distance);
OCT_local OCT_rope2D_constrain(OCT_local entityA, OCT_local entityB, OCT_vec2 anchorA, OCT_vec2 anchorB, float length);