#include "textureGroup_int.h"
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer/renderer_int.h"

#define TEXTURE_BASE_LEVEL 0
#define TEMP_MIPMAPS 1

OCT_global OCT_textureGroup_open(OCT_vec2 pixelDimensions, OCT_index maxCount) {
    OCT_ID systemID = iOCT_renderer_inst.systemID;
    eOCT_IDMap texMap = eOCT_IDMap_open(systemID, maxCount);

    GLuint texArray;
    glGenTextures(1, &texArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, TEXTURE_BASE_LEVEL, GL_RGBA8, (GLsizei)pixelDimensions.x, (GLsizei)pixelDimensions.y, (GLsizei)maxCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // params in opengl wiki example
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    // add to texture group pool
    iOCT_textureGroup newTexGroup = {
        .dimensions = pixelDimensions,
        .glTexArray = texArray,
        .textureMap = texMap,
        .textureCount = 0
    };
    // iOCT_textureGroup* newTexGroupLoc;
    OCT_ID newID;
    eOCT_mappedPool_addEntry(&iOCT_renderer_inst.textureGroupMPool, &newTexGroup, &newID, NULL);

    OCT_global newHandle = {
        .systemID = iOCT_renderer_inst.systemID,
        .objectID = newID
    };

    // printf("Created new texture group\n");

    return newHandle;
}

OCT_global OCT_texture_new(OCT_global textureGroup, const char* path) {
    const unsigned char* pixels = eOCT_image_load(path);
    if (!pixels) {
        perror("Can't open");
        OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_OPEN_FILE, "Failed to load image");
        return OCT_GLOBAL_NULL;
    }

    // eOCT_IDMap* groupMap = &iOCT_renderer_inst.textureGroupMap;
    // eOCT_pool* groupPool = &iOCT_renderer_inst.textureGroupPool;
    // iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_getByID(groupMap, groupPool, textureGroup.objectID);
    iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_mappedPool_getByID(&iOCT_renderer_inst.textureGroupMPool, textureGroup.objectID);

    OCT_index newTexLayer = texGroup->textureCount++;
    glBindTexture(GL_TEXTURE_2D_ARRAY, texGroup->glTexArray);

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, TEXTURE_BASE_LEVEL,
                    0, 0, (GLsizei)newTexLayer,
                    (GLsizei)texGroup->dimensions.x, (GLsizei)texGroup->dimensions.y, 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    OCT_ID newTexID = eOCT_IDMap_register(&texGroup->textureMap, newTexLayer);

    OCT_global newHandle = {
        .objectID = newTexID,
        .containerID = texGroup->textureGroupID,
        .systemID = iOCT_renderer_inst.systemID
    };

    // printf("Loaded new texture\n");

    return newHandle;
}