#pragma once
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"

#include "renderer/renderer_int.h"

struct iOCT_camera2D {
    OCT_local entityHandle;

    OCT_vec2 position;
    float rotation;
    float zoom;
    OCT_vec2 viewFrameSize;

    OCT_mat3 cameraMatrix;
};

eOCT_DEFINE_COMPONENT_ACCESSOR(iOCT_camera2D, iOCT_renderer_inst, camera2DKey)