#include "renderer_int.h"
#include "types_int.h"

#include <stdio.h>
#include <glad/glad.h>
#include <assert.h>

#include "texture/textureGroup_int.h"
#include "renderer/textures.h"
#include "sprite2D/sprite2D_int.h"
#include "shader/shader_int.h"
#include "window/windowSystem_int.h"
#include "window/window/window_int.h"

#define iOCT_SPRITES_INITIAL_CAPACITY 16
#define iOCT_SPRITES_EXPANSION_FACTOR 2
#define iOCT_QUAD_INDEX_CT 6

typedef enum iOCT_spriteAttributes {
    attrib_quadXY,
    attrib_quadUV,
    attrib_uv,
    attrib_color,
    attrib_dimensions,
    attrib_arrayLayer,
    attrib_transformCol0,
    attrib_transformCol1,
    attrib_transformCol2,
} iOCT_spriteAttributes;
iOCT_renderer iOCT_renderer_inst = { 0 };

// basic quad shape
typedef struct quadVertex {
    float x;
    float y;
    float u;
    float v;
} quadVertex;
static quadVertex spriteVertices[] = {
    {-0.5f,  0.5f,     0.0f, 1.0f}, // top left
    { 0.5f,  0.5f,     1.0f, 1.0f}, // top right
     {0.5f, -0.5f,     1.0f, 0.0f}, // bottom right
    {-0.5f, -0.5f,     0.0f, 0.0f}  // bottom left
};
static unsigned int spriteIndices[] = {
    0, 1, 2,
    2, 3, 0,
};

static void iOCT_setInstanceOffset(GLuint VAO, GLuint VBO, size_t byteOffset);
static OCT_global iOCT_initGizmoTex();

void system_init_RENDERER() {
    OCT_ID systemID = iOCT_renderer_inst.systemID;
    // iOCT_renderer_inst.textureGroupPool = eOCT_pool_open(systemID, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_textureGroup));
    // iOCT_renderer_inst.textureGroupMap = eOCT_IDMap_open(systemID, eOCT_POOL_CAPACITY_DEFAULT);
    iOCT_renderer_inst.textureGroupMPool = eOCT_mappedPool_open(systemID, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_textureGroup), offsetof(iOCT_textureGroup, textureGroupID));
    iOCT_renderer_inst.spriteFullDataBuffer = eOCT_pool_open(systemID, eOCT_POOL_CAPACITY_DEFAULT,sizeof(iOCT_spriteFullData));

    GLuint spriteVAO;
    glGenVertexArrays(1, &spriteVAO);
    glBindVertexArray(spriteVAO);

#pragma region basic quad layout
    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);
    // quad xy
    glVertexAttribPointer(attrib_quadXY, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // quad uv
    glVertexAttribPointer(attrib_quadUV, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint quadEBO;
    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spriteIndices), spriteIndices, GL_STATIC_DRAW);
#pragma endregion

#pragma region uploaded data layout
    GLuint spriteDataVBO;
    glGenBuffers(1, &spriteDataVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spriteDataVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(iOCT_spriteFullData) * iOCT_SPRITES_INITIAL_CAPACITY, iOCT_renderer_inst.spriteFullDataBuffer.array, GL_DYNAMIC_DRAW);

    // glVertexAttribPointer(attrib_uv, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, uv))); //uv
    // glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, color)));
    // glVertexAttribPointer(attrib_dimensions, 2, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, dimensions)));
    // glVertexAttribPointer(attrib_arrayLayer, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, texArrayLayer)));
    // glVertexAttribPointer(attrib_transformCol0, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c0r0)));
    // glVertexAttribPointer(attrib_transformCol1, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c1r0)));
    // glVertexAttribPointer(attrib_transformCol2, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c2r0)));
    iOCT_renderer_inst.spriteVAO = spriteVAO;
    iOCT_renderer_inst.quadEBO = quadEBO;
    iOCT_renderer_inst.quadVBO = quadVBO;
    iOCT_renderer_inst.spriteDataVBO = spriteDataVBO;
    iOCT_setupNewSpriteVAO(iOCT_renderer_inst.spriteVAO);

#pragma endregion

#pragma region shaders
    GLuint spriteShader = iOCT_shader_createProgram(
        "OCTAVIANEngine/Renderer/internal/renderer/shader/sprite/sprite.vert",
        "OCTAVIANEngine/Renderer/internal/renderer/shader/sprite/sprite.frag");
    glUseProgram(spriteShader);
    glUniform1i(glGetUniformLocation(spriteShader, "spriteTexArray"), 0); // read from texture slot 0
    GLint cameraUniform = glGetUniformLocation(spriteShader, "cameraProj");
    glUniformMatrix3fv(cameraUniform, 1, GL_FALSE, (float*)&OCT_mat3_identity);

    glUseProgram(0);
#pragma endregion

    iOCT_renderer_inst.cameraUniform = cameraUniform;
    iOCT_renderer_inst.spriteDataVBOCapacity = iOCT_SPRITES_INITIAL_CAPACITY;
    iOCT_renderer_inst.spriteShaderProgram = spriteShader;
    iOCT_renderer_inst.gizmoTex = iOCT_initGizmoTex();

    glBindVertexArray(0);
}

void iOCT_renderer_contextSetup(OCT_global context) {
    bool* screenSpace = &eOCT_single_getLocal(iOCT_renderer_inst.screenSpaceKey, context)->boolean;
    *screenSpace = false;
}

/*!
 * Uploads all sprite data to the sprite VBO. Skips uploading hidden sprites completely.
 * @param contextHandle
 */
void iOCT_renderer_uploadAll(OCT_global contextHandle) {
    eOCT_pool_clear(&iOCT_renderer_inst.spriteFullDataBuffer);

    // Source
    // eOCT_pool* spritePool = eOCT_context_getComponentPool(contextHandle, iOCT_renderer_inst.sprite2DCache);
    eOCT_pool* spritePool = eOCT_component_getPool(contextHandle, iOCT_renderer_inst.sprite2DKey);
    iOCT_sprite2D* spriteArray = (iOCT_sprite2D*)spritePool->array;
    // Buffer
    eOCT_pool* spriteBufferPool = &iOCT_renderer_inst.spriteFullDataBuffer;
    if (spritePool->count > spriteBufferPool->capacity) {
        eOCT_pool_expand(spriteBufferPool, spritePool->count);
    }

    eOCT_contextToken contextToken = eOCT_context_getToken(contextHandle);
    for (OCT_index spriteCtr = 0; spriteCtr < spritePool->count; spriteCtr++) {
        iOCT_sprite2D sprite = spriteArray[spriteCtr];
        if (sprite.visible == false) {
            continue;
        }
        // iOCT_textureGroup texGroup = *(iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, sprite.texGroupID);
        iOCT_textureGroup texGroup = *(iOCT_textureGroup*)eOCT_mappedPool_getByID(&iOCT_renderer_inst.textureGroupMPool, sprite.texGroupID);
        OCT_index texArrayLayer = eOCT_IDMap_getIndex(&texGroup.textureMap, sprite.texID);

        // resolve final transform
        iOCT_spriteData spriteData = spriteArray[spriteCtr].spriteData;
        OCT_mat3* entityTransformPtr = (OCT_mat3*)eOCT_entity_getFieldByToken(contextToken, sprite.entityHandle, iOCT_renderer_inst.transform2DTicket);
        OCT_mat3 entityTransform;
        if (entityTransformPtr == NULL) {
            entityTransform = OCT_mat3_identity;
            printf("Has no transform\n");
        }
        else {
            entityTransform = *entityTransformPtr;
        }
        OCT_mat3 finalTransform = OCT_mat3_mul(entityTransform, sprite.spriteTransform);

        spriteData.texArrayLayer = (GLuint)texArrayLayer;

        // batch a single sprite's data
        iOCT_spriteFullData spriteFullData = {
            .spriteData = spriteData,
            .transform = finalTransform
        };
        eOCT_pool_addEntryNew(spriteBufferPool, &spriteFullData, NULL);
        // iOCT_spriteFullData* fullData = (iOCT_spriteFullData*)eOCT_pool_addEntry(spriteBufferPool, NULL);
        // fullData->spriteData = spriteData;
        // fullData->transform = finalTransform;

        // printf("Uploaded sprite #%zu with final transform from entity with ID %d\n", spriteCtr, sprite.entityID);
        // OCT_mat3_print(finalTransform);
        // printf("Matrix source: %p\n", entityTransformPtr);
    }
    eOCT_context_invalidateToken(&contextToken);

    // expand buffer if necessary, then upload
    glBindBuffer(GL_ARRAY_BUFFER, iOCT_renderer_inst.spriteDataVBO);

    bool resized = false;
    while (spriteBufferPool->count > iOCT_renderer_inst.spriteDataVBOCapacity) {
        assert(iOCT_SPRITES_EXPANSION_FACTOR > 1 && iOCT_renderer_inst.spriteDataVBOCapacity > 0);
        iOCT_renderer_inst.spriteDataVBOCapacity *= iOCT_SPRITES_EXPANSION_FACTOR;
        resized = true;
    }
    if (resized) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(iOCT_spriteFullData) * iOCT_renderer_inst.spriteDataVBOCapacity, spriteBufferPool->array, GL_DYNAMIC_DRAW);
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, spriteBufferPool->count * spriteBufferPool->elementSize, spriteBufferPool->array);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*!
 * Draws all sprites from the sprite VBO. Batches by layer, then by textureGroup within each layer.
 * @param contextHandle
 */
void iOCT_renderer_drawAll(OCT_global contextHandle) {
    eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowMPool.pool;
    iOCT_window* windowArray = (iOCT_window*)windowPool->array;
    eOCT_pool* spritePool = eOCT_component_getPool(contextHandle, iOCT_renderer_inst.sprite2DKey);
    iOCT_sprite2D* spriteArray = (iOCT_sprite2D*)spritePool->array;
    bool screenSpace = eOCT_single_getLocal(iOCT_renderer_inst.screenSpaceKey, contextHandle)->boolean;

    assert(windowPool && windowArray && spritePool && spriteArray && "Renderer data grab failed\n");

    if (spritePool->count == 0) {
        return;
    }

    // for each window, draw everything based on its camera
    for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
        // per window (context)
        iOCT_window window = windowArray[windowCtr];
        iOCT_window_activate(window);
        glUseProgram(iOCT_renderer_inst.spriteShaderProgram);

        // camera
        if (OCT_local_isEqual(window.activeCameraSourceEntity, OCT_LOCAL_NULL)) {
            printf("Window %zu has no active camera\n", windowCtr);
            continue;
        }
        OCT_mat3 cameraProj;
        if (screenSpace) {
            cameraProj = OCT_mat3_inv(OCT_mat3_scale(OCT_mat3_identity, OCT_vec2_mul(window.screenSpaceZoom, 0.5)));
        } else {
            cameraProj = iOCT_window_worldToNDC(window);
        }

        glUniformMatrix3fv(window.cameraUniformLocation, 1, GL_FALSE, (float*)&cameraProj);

        OCT_index spriteCtr = 0;
        GLsizei drawnCt = 0;
        // draw all sprites batched
        while (spriteCtr < spritePool->count) {
            // note the current batch
            iOCT_sprite2D sprite = spriteArray[spriteCtr];  // first sprite of the batch
            OCT_index currentLayer = sprite.drawLayer;
            OCT_ID currentTexGroup = sprite.texGroupID;
            GLsizei batchCt = 0;

            // increment until a different batch is hit
            while (spriteCtr < spritePool->count &&
                   sprite.drawLayer == currentLayer &&
                   sprite.texGroupID == currentTexGroup) {
                if (sprite.visible) {
                    batchCt++;
                }
                spriteCtr++;
                if (spriteCtr >= spritePool->count) {
                    break;
                }
                sprite = spriteArray[spriteCtr];
            }

            iOCT_textureGroup texGroup = *(iOCT_textureGroup*)eOCT_mappedPool_getByID(&iOCT_renderer_inst.textureGroupMPool, currentTexGroup);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, texGroup.glTexArray);

            // create the offset for drawElementsInstanced
            size_t drawnByteOffset = drawnCt * iOCT_renderer_inst.spriteFullDataBuffer.elementSize;
            iOCT_setInstanceOffset(iOCT_renderer_inst.spriteVAO, iOCT_renderer_inst.spriteDataVBO, drawnByteOffset);
            glDrawElementsInstanced(GL_TRIANGLES, iOCT_QUAD_INDEX_CT, GL_UNSIGNED_INT, (void*)0, batchCt); // void0 is not buffer offset

            drawnCt += batchCt;
        }
    }
}

void iOCT_setupNewSpriteVAO(GLuint VAO) {
    glBindVertexArray(VAO);

    // quad VBO
    glBindBuffer(GL_ARRAY_BUFFER, iOCT_renderer_inst.quadVBO);
    glVertexAttribPointer(attrib_quadXY, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // quad uv
    glVertexAttribPointer(attrib_quadUV, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // quad EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iOCT_renderer_inst.quadEBO);

    // spriteVBO
    glBindBuffer(GL_ARRAY_BUFFER, iOCT_renderer_inst.spriteDataVBO);
    iOCT_setInstanceOffset(VAO, iOCT_renderer_inst.spriteDataVBO, 0);
    glEnableVertexAttribArray(attrib_uv);
    glEnableVertexAttribArray(attrib_color);
    glEnableVertexAttribArray(attrib_dimensions);
    glEnableVertexAttribArray(attrib_arrayLayer);
    glEnableVertexAttribArray(attrib_transformCol0);
    glEnableVertexAttribArray(attrib_transformCol1);
    glEnableVertexAttribArray(attrib_transformCol2);
    glVertexAttribDivisor(attrib_uv, 1);
    glVertexAttribDivisor(attrib_color, 1);
    glVertexAttribDivisor(attrib_dimensions, 1);
    glVertexAttribDivisor(attrib_arrayLayer, 1);
    glVertexAttribDivisor(attrib_transformCol0, 1);
    glVertexAttribDivisor(attrib_transformCol1, 1);
    glVertexAttribDivisor(attrib_transformCol2, 1);
}

void eOCT_RENDERER_update(OCT_global contextHandle) {
    iOCT_renderer_uploadAll(contextHandle);
    iOCT_renderer_drawAll(contextHandle);
}

static void iOCT_setInstanceOffset(GLuint VAO, GLuint VBO, size_t byteOffset) {
    size_t spriteDataBase = offsetof(iOCT_spriteFullData, spriteData);
    size_t transformBase = offsetof(iOCT_spriteFullData, transform);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(attrib_uv, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, uv) + byteOffset)); //uv
    glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, color) + byteOffset));
    glVertexAttribPointer(attrib_dimensions, 2, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, dimensions) + byteOffset));
    glVertexAttribPointer(attrib_arrayLayer, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, texArrayLayer) + byteOffset));
    glVertexAttribPointer(attrib_transformCol0, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c0r0) + byteOffset));
    glVertexAttribPointer(attrib_transformCol1, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c1r0) + byteOffset));
    glVertexAttribPointer(attrib_transformCol2, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c2r0) + byteOffset));
}

static OCT_global iOCT_initGizmoTex() {
    OCT_global systemTexGroup = OCT_textureGroup_open(iOCT_SYSTEMTEX_DIMENSIONS, 1);
    OCT_global gizmoTex = OCT_texture_new(systemTexGroup, "../OCTAVIAN/OCTAVIANEngine/Renderer/resources/frameGizmo.png"); // __NOTE__ CHANGE PATH METHOD
    return gizmoTex;
}

