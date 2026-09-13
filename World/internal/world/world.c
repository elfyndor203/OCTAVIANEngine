#include "world_int.h"
#include "types_int.h"

#include "transform2D/transform2D_int.h"

iOCT_world iOCT_world_inst = { 0 };

void iOCT_world_init() {
}

void eOCT_WORLD_update(OCT_global contextHandle) {
    // eOCT_pool* transformPool = (eOCT_pool*)eOCT_component_getPool(contextHandle, iOCT_world_inst.transform2DKey);
    eOCT_pool* transformPool = iOCT_transform2D_getPool(contextHandle);
    iOCT_transform2D* transformArray = (iOCT_transform2D*)transformPool->array;

    iOCT_transform2D_propagate(contextHandle);

    // for (OCT_index transformCtr = 0; transformCtr < transformPool->count; transformCtr++) {
    //     iOCT_transform2D* transform = &transformArray[transformCtr];
    //
    // }
}