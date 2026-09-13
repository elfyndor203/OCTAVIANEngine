#pragma once

#include "ECS/entityContexts/entityContexts_int.h"
#include <inttypes.h>
#include <stdbool.h>

struct iOCT_entityMeta {
    OCT_local entity;

    uint64_t componentsAttached;
    uint64_t componentsEnabled;
    bool isRoot;
};

OCT_local iOCT_entity_new(iOCT_entityContext* context);
OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);
// void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component, bool sort, OCT_index sortValue);
void iOCT_entity_attachMeta(OCT_local entity);
void iOCT_entity_updateAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB attachOrDetach);
void iOCT_entity_updateEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex, OCT_AorB enableOrDisable);
bool iOCT_entity_readAttachedMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex);
bool iOCT_entity_readEnabledMask(iOCT_entityMeta* entityMeta, OCT_index componentIndex);
/*!
 * Re-resolves all indices stored in entities. Run after any component pool shuffling.
 * @param context
 * @param componentPool
 * @param component
 */
void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentKey component, OCT_index skip);
