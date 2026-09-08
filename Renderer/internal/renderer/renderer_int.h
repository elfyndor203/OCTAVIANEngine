#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>

#define GL_CHECK() { GLenum err = glGetError(); if (err != GL_NO_ERROR) printf("GL error %d at line %d\n", err, __LINE__); }
#define iOCT_SYSTEMTEX_DIMENSIONS ((OCT_vec2){100, 100})

struct iOCT_renderer {
    // system information
    OCT_ID systemID;

    eOCT_fieldTicket transform2DTicket;
    eOCT_fieldTicket windowVAOCache;
    eOCT_componentKey sprite2DKey;
    eOCT_componentKey camera2DKey;
    eOCT_singleKey screenSpaceKey;
    // eOCT_singleKey screenSpaceZoomKey;

    // texture storage
    // eOCT_IDMap textureGroupMap;
    // eOCT_pool textureGroupPool;
    eOCT_mappedPool textureGroupMPool;

    // openGL sprite drawing
    eOCT_pool spriteFullDataBuffer;
    GLuint spriteVAO;
    GLuint quadVBO;
    GLuint quadEBO;
    GLuint spriteDataVBO;
    OCT_index spriteDataVBOCapacity;
    GLuint spriteShaderProgram;

    // provided at init
    OCT_global gizmoTex;

    // uniforms
    GLint cameraUniform;
};

extern iOCT_renderer iOCT_renderer_inst;

void system_init_RENDERER();

void iOCT_renderer_drawLayer();
void iOCT_setupNewSpriteVAO(GLuint VAO);
void iOCT_renderer_contextSetup(OCT_global context);
