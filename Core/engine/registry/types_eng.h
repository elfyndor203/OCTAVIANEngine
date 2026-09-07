#pragma once
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

/// <summary>
/// Describes one component provided by the system. 
/// Leave all _reg fields blank.
/// Do not include private fields.
/// { "name", sizeof(component), eOCT_generateFieldDescriptionPool(publicFields[], count ))
/// </summary>
typedef struct eOCT_componentDescription eOCT_componentDescription;

typedef struct eOCT_dataPoolDescription eOCT_dataPoolDescription;
typedef struct eOCT_eventDescription eOCT_eventDescription;
typedef struct eOCT_singleDescription eOCT_singleDescription;

typedef union eOCT_dataUnion eOCT_dataUnion;

typedef void (*eOCT_systemInitFx)(void);
typedef void (*eOCT_contextInitFx)(OCT_global context);

typedef struct eOCT_fieldTicket eOCT_fieldTicket;
typedef struct eOCT_componentExistenceKey eOCT_componentExistenceKey;
// typedef struct eOCT_fieldTicket_global eOCT_fieldTicket_global;
typedef struct eOCT_componentKey eOCT_componentKey;
typedef struct eOCT_eventKey eOCT_eventKey;
typedef struct eOCT_dataPoolKey eOCT_dataPoolKey;
typedef struct eOCT_singleKey eOCT_singleKey;
