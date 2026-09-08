#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "world/world_int.h"

#define iOCT_TRANSFORM_ROOT_DEPTH 0


struct iOCT_transform2D {
	OCT_local entityHandle;

	OCT_vec2 position;
	float rotation;
	OCT_vec2 scale;

	OCT_local parentEntityHandle;
	OCT_index depth;

	OCT_mat3 localMatrix;
	OCT_mat3 globalMatrix;
};

OCT_DEFINE_COMPONENT_ACCESSOR(iOCT_transform2D, iOCT_world_inst, transform2DKey)
void iOCT_transform2D_generateRoot(OCT_local rootEntity);
void iOCT_transform2D_propagate(OCT_global context);

