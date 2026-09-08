#pragma once
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>

#include "renderer/renderer_int.h"

struct iOCT_spriteData {
    OCT_vec4 uv;
    OCT_vec4 color;
    OCT_vec2 dimensions;
    GLuint texArrayLayer;
};

struct iOCT_spriteFullData {
    iOCT_spriteData spriteData;
    OCT_mat3 transform;
};

struct iOCT_sprite2D {
    OCT_local entityHandle;

    OCT_ID windowID;
    OCT_index drawLayer;
    OCT_ID texGroupID;
    OCT_ID texID;
    OCT_index sortKey;
    bool visible;

    OCT_mat3 spriteTransform;
    iOCT_spriteData spriteData;
};

eOCT_DEFINE_COMPONENT_ACCESSOR(iOCT_sprite2D, iOCT_renderer_inst, sprite2DKey)
void iOCT_sprite2D_root(OCT_local rootEntity);
