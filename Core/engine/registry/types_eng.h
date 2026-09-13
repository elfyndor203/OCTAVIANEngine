#pragma once

#include "layout/types.h"
/// <summary>
/// Describes one system with its provided components, public fields, and requested fields. 
/// Leave all _reg fields blank.
/// { "name", eOCT_generateComponentDescriptionPool(components[], count), eOCT_generateFieldRequestPool(requests[], count) }
/// </summary>
typedef struct eOCT_systemDescription eOCT_systemDescription;

/// <summary>
/// Describes one public field in a component provided by the system.
/// Leave all _reg fields blank.
/// Do not describe private fields.
/// { "name", eOCT_FIELDTYPE, offsetof(field) }
/// </summary>
typedef struct eOCT_fieldDescription eOCT_fieldDescription;

/// <summary>
/// Describes one field the system is requesting from another system. The name field must match the other system.
/// Leave all _reg fields blank.
/// { "name", eOCT_FIELDTYPE, optional T/F };
/// </summary>
typedef struct eOCT_fieldRequest eOCT_fieldRequest;

typedef struct eOCT_componentExistenceKey eOCT_componentExistenceKey;



typedef union eOCT_dataUnion eOCT_dataUnion;

typedef void (*eOCT_systemInitFx)(void);
typedef void (*eOCT_contextInitFx)(OCT_global context);

typedef struct eOCT_fieldTicket eOCT_fieldTicket;
typedef struct eOCT_componentExistenceKey eOCT_componentExistenceKey;
// typedef struct eOCT_fieldTicket_global eOCT_fieldTicket_global;

