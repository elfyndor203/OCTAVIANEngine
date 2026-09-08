#include "sprite2D_int.h"
#include "renderer/sprite2D.h"
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

#include "renderer/renderer_int.h"
#include "renderer/colors.h"

#define iOCT_LAYER_MAX (UINT32_MAX - 1)

uint64_t generateSortKey(OCT_index drawLayer, OCT_index texGroupIndex);

void OCT_sprite2D_attach(OCT_local entity, OCT_global texture, OCT_vec4 uv, OCT_vec2 dimensions, OCT_index drawLayer) {
    // OCT_index texGroupIndex = eOCT_IDMap_getIndex(&iOCT_renderer_inst.textureGroupMap, texture.containerID);
    OCT_index texGroupIndex = eOCT_IDMap_getIndex(&iOCT_renderer_inst.textureGroupMPool.IDMap, texture.containerID);
    iOCT_sprite2D newSprite = {
        .entityHandle = entity,
        .texGroupID = texture.containerID,
        .texID = texture.objectID,
        .spriteTransform = OCT_mat3_identity,
        .sortKey = generateSortKey(drawLayer, texGroupIndex),
        .visible = true,
        .spriteData = {
            .uv = uv,
            .color = OCT_COLOR_WHITE,
            .dimensions = dimensions,
        }
    };
    eOCT_entity_attachComponent(entity, iOCT_renderer_inst.sprite2DKey, &newSprite, NULL);
    // iOCT_sprite2D* newSprite = eOCT_entity_attachComponent(entity, iOCT_renderer_inst.sprite2DKey);
    // newSprite->entityHandle = entity;
    // newSprite->texGroupID = texture.containerID;
    // newSprite->texID = texture.objectID;
    // newSprite->spriteTransform = OCT_mat3_identity;
    // // resolve spriteData texArrayLayer at draw time
    // newSprite->spriteData.uv = uv;
    // newSprite->spriteData.color = tintColor;
    // newSprite->spriteData.dimensions = dimensions;
    // newSprite->sortKey = generateSortKey(drawLayer, texGroupIndex);

    printf("Attached sprite2D to entity %zu\n", entity.objectID);

    // iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, texture.containerID);
}

void OCT_sprite2D_hide(OCT_local entity) {
    iOCT_sprite2D* sprite = iOCT_sprite2D_get(entity);
    sprite->visible = false;
}
void OCT_sprite2D_show(OCT_local entity) {
    iOCT_sprite2D* sprite = iOCT_sprite2D_get(entity);
    sprite->visible = true;
}
/// assumes layer and texGroup don't exceed 16 bit max, because it'd better not
uint64_t generateSortKey(OCT_index drawLayer, OCT_index texGroupIndex) {
    assert(drawLayer <= UINT32_MAX);
    assert(texGroupIndex <= UINT32_MAX);

    uint32_t layerBits = (uint32_t)drawLayer;
    uint32_t texBits = (uint32_t)texGroupIndex;

    return ((uint64_t)layerBits << 32) | (uint64_t)texBits;
}

void iOCT_sprite2D_root(OCT_local rootEntity) {
    OCT_sprite2D_attach(rootEntity, iOCT_renderer_inst.gizmoTex, (OCT_vec4){0.0, 0.0, 1.0, 1.0}, (OCT_vec2){100.0f, 100.0f}, iOCT_LAYER_MAX); // skips layers in between
}

