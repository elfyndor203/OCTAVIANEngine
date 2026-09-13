#include "components_int.h"

#include "../../entities/entities_int.h"

void* iOCT_component_get(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {	// actual access logic
    OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
    OCT_index* componentIndexBase = entityBase + componentTypeIndex;
    OCT_index componentIndex = *componentIndexBase;

    // printf("Component index: %zu\n", componentIndex);

    if (componentIndex == OCT_INDEX_NULL) {
        return NULL;
    }

    eOCT_pool* componentPool = iOCT_context_getComponentPool(context, componentTypeIndex);
    void* dataLoc = eOCT_pool_access(componentPool, componentIndex, 0);
    return dataLoc;
}

OCT_index* iOCT_component_getEntitySlot(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component) { // __NOTE__ copied into eOCT_component_getField, maybe fix
    if (!context) {
        OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
    }
    OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
    OCT_index* componentSlot = entityBase + component.componentTypeIndex;

    return componentSlot;
}