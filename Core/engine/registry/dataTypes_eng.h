#pragma once

#define eOCT_DATAUNION_SIZE 36

typedef enum eOCT_dataPattern {
    eOCT_DATAPATTERN_COMPONENT = 1,
    eOCT_DATAPATTERN_EVENT,
    eOCT_DATAPATTERN_DATAPOOL,
    eOCT_DATAPATTERN_SINGLE
} eOCT_dataPattern;

typedef enum eOCT_dataTypes { //__NOTE__ typedef all types? or no
    eOCT_TYPE_NULL = 0,
    eOCT_TYPE_INT64,
    eOCT_TYPE_UINT64,
    eOCT_TYPE_FLOAT32,
    eOCT_TYPE_DOUBLE64,
    eOCT_TYPE_CHAR8,
    eOCT_TYPE_PTR64,
    eOCT_TYPE_STRING64,
    eOCT_TYPE_BOOL8, // size? include all sizes or just specific?

    eOCT_TYPE_HANDLE_LOCAL,
    eOCT_TYPE_HANDLE_GLOBAL,
    eOCT_TYPE_ID,
    eOCT_TYPE_INDEX,
    eOCT_TYPE_VEC2,
    eOCT_TYPE_VEC3,
    eOCT_TYPE_VEC4,
    eOCT_TYPE_MAT3,
    eOCT_TYPE_MAT4,

    eOCT_TYPE_CUSTOM	// Must have a size of <= eOCT_DATAUNION_SIZE. Redefine if necessary. Won't be readable by other systems. Should be used for external API data that needs to be context-local.
} eOCT_dataTypes;

union eOCT_dataUnion {
    int int64;
    uint64_t uint64;
    float float32;
    double double64;
    char char8;
    void* ptr8;
    char* string8;
    bool boolean;
    OCT_ID ID;
    OCT_index index;
    OCT_vec2 vec2;
    OCT_vec3 vec3;
    OCT_vec4 vec4;
    OCT_mat3 mat3;

    char opaque[eOCT_DATAUNION_SIZE];
};