#pragma once
#include "layout/types.h"

OCT_local OCT_entity_new(OCT_global contextHandle);
bool OCT_entity_sameContext(OCT_local entity1, OCT_local entity2);
bool OCT_entity_fromContext(OCT_local entity, OCT_local context);

void OCT_entity_printAllComponentIndices(OCT_global contextHandle);