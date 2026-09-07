#pragma once

#define eOCT_DATAUNION_SIZE 36

typedef enum eOCT_dataPattern {
    eOCT_DATAPATTERN_COMPONENT = 1,
    eOCT_DATAPATTERN_EVENT,
    eOCT_DATAPATTERN_DATAPOOL,
    eOCT_DATAPATTERN_SINGLE
} eOCT_dataPattern;

typedef enum eOCT_dataTypes { //__NOTE__ typedef all types? or no
    eOCT_DATATYPE_NULL = 0,
    eOCT_DATATYPE_INT64,
    eOCT_DATATYPE_UINT64,
    eOCT_DATATYPE_FLOAT32,
    eOCT_DATATYPE_DOUBLE64,
    eOCT_DATATYPE_CHAR8,
    eOCT_DATATYPE_PTR64,
    eOCT_DATATYPE_STRING64,
    eOCT_DATATYPE_BOOL, // size? include all sizes or just specific?

    eOCT_DATATYPE_HANDLE_LOCAL,
    eOCT_DATATYPE_HANDLE_GLOBAL,
    eOCT_DATATYPE_ID,
    eOCT_DATATYPE_INDEX,
    eOCT_DATATYPE_VEC2,
    eOCT_DATATYPE_VEC3,
    eOCT_DATATYPE_VEC4,
    eOCT_DATATYPE_MAT3,
    eOCT_DATATYPE_MAT4,

    eOCT_DATATYPE_CUSTOM	// Must have a size of <= eOCT_DATAUNION_SIZE. Redefine if necessary. Won't be readable by other systems. Should be used for external API data that needs to be context-local.
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