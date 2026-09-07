#pragma once
#include "types_eng.h"

#include "dataTypes_eng.h"
#include "layout/types.h"
#include "utilities/utilities_eng.h"

struct eOCT_fieldDescription {
    const char* name;
    eOCT_dataTypes type;	// standard field types defined in fields.h
    size_t offset;			// offset from the start of the component struct
    eOCT_dataPattern providerType;

    OCT_index providerIndex_reg;
    bool global_reg;
};

struct eOCT_fieldRequest {
    const char* name;
    eOCT_dataTypes type;
    eOCT_fieldTicket* ticketCache;
    eOCT_dataPattern providerType;
    bool optional;
    const char* groupName_opt;

    OCT_index providerIndex_reg;
    size_t fieldOffset_reg;
    bool global_reg;
    bool fulfilled_reg;
    int groupStatus_reg;
};

struct eOCT_fieldTicket {
    const char* name;		// for debugging
    eOCT_dataTypes type;
    size_t offsetFromStruct;
    eOCT_dataPattern providerType;
    OCT_index providerTypeIndex;

    bool global;
    eOCT_pool* globalPool;
};

struct eOCT_requestGroup {
    eOCT_pool fieldRequests;
    eOCT_dataPattern providerType;
    OCT_index providerIndex_reg;
};

