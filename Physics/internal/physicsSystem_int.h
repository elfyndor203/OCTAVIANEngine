#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include <box2d/box2d.h>

struct iOCT_physicsSystem {
    OCT_ID systemID;

    eOCT_componentKey physics2DKey;
    eOCT_dataPoolKey distance2DKey;
    eOCT_dataPoolKey collider2DKey;
    eOCT_fieldTicket globalMatrix2DTicket;
    eOCT_fieldTicket position2DTicket;
    eOCT_fieldTicket rotationTicket;
    eOCT_fieldTicket transformParentTicket;

    eOCT_singleKey box2DWorldKey;

    OCT_vec2 worldGravity;
    double dt;
    OCT_index constraintSolveIterations;

    float unitsPerB2Meter;
};

extern iOCT_physicsSystem iOCT_physicsSystem_inst;

eOCT_DEFINE_COMPONENT_FIELD_ACCESSOR(iOCT_globalMatrix2D, OCT_mat3, iOCT_physicsSystem_inst, globalMatrix2DTicket)
eOCT_DEFINE_COMPONENT_FIELD_ACCESSOR(iOCT_transformParent, OCT_local, iOCT_physicsSystem_inst, transformParentTicket)
void iOCT_physicsSystem_init();
void iOCT_physicsSystem_contextSetup(OCT_global context);

b2Vec2 iOCT_toB2Vec2(OCT_vec2);
OCT_vec2 iOCT_toOCTVec2(b2Vec2 b2Vec2);