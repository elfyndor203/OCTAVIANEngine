#include "fields_eng.h"

#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdarg.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContext_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"

eOCT_pool eOCT_generateFieldDescriptionPool(OCT_index total, eOCT_fieldDescription description1, ...) {
	if (total < 1) {
		OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no fields are provided");
		return eOCT_POOL_EMPTY;
	}
	va_list args;
	va_start(args, description1);

	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_fieldDescription));
	bool end = false;
	OCT_index processed = 0;
	eOCT_fieldDescription newRequest = description1;
	while (!end && processed < total) {
		if (strcmp(newRequest.name, eOCT_END_FIELDS.name) == 0) { // checks for END flag
			end = true;

			if (processed != total) {										// END flag should be after all requests are processed
				OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less fields provided than expected");
				return pool;
			}
		} else {
			eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
			processed++;
			newRequest = va_arg(args, eOCT_fieldDescription);
		}
	}
	eOCT_fieldDescription expectedEnd = newRequest;	// most recent: either the END flag or error
	if (strcmp(expectedEnd.name, eOCT_END_FIELDS.name) != 0) {
		OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
		return pool;
	}

	va_end(args);
	return pool;
}

eOCT_pool eOCT_generateFieldRequestPool(OCT_index total, eOCT_fieldRequest request1, ...) {
	if (total < 1) {
		OCT_ERROR_LOG(OCT_WARNING_IMPROPER, "Directly pass empty pool if no fields are requested");
		return eOCT_POOL_EMPTY;
	}
	va_list args;
	va_start(args, request1);

	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, total, sizeof(eOCT_fieldRequest));
	bool end = false;
	OCT_index processed = 0;
	eOCT_fieldRequest newRequest = request1;
	while (!end && processed < total) {
		if (strcmp(newRequest.name, eOCT_END_REQUESTS.name) == 0) { // checks for END flag
			end = true;

			if (processed != total) {										// END flag should be after all requests are processed
				OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Less fields provided than expected");
				return pool;
			}
		} else {
			eOCT_pool_addEntryNew(&pool, &newRequest, NULL);
			processed++;
			newRequest = va_arg(args, eOCT_fieldRequest);
		}
	}
	eOCT_fieldRequest expectedEnd = newRequest;	// most recent: either the END flag or error
	if (strcmp(expectedEnd.name, eOCT_END_REQUESTS.name) != 0) {
		OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "END flag not found");
		return pool;
	}

	va_end(args);
	return pool;
}

eOCT_pool* eOCT_field_getSourcePool(OCT_global contextHandle, eOCT_fieldTicket fieldTicket) {
	if (fieldTicket.global && fieldTicket.globalPool) {
		return fieldTicket.globalPool;
	}

	if (fieldTicket.providerType == eOCT_DATAPATTERN_COMPONENT) {
		iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
		eOCT_pool* componentPool = &((eOCT_pool*)context->components.array)[fieldTicket.providerTypeIndex];
		return componentPool;
	}

	if (fieldTicket.providerType == eOCT_DATAPATTERN_DATAPOOL) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Data pools are being updated\n");
		return NULL;
	}

	if (fieldTicket.providerType == eOCT_DATAPATTERN_EVENT) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
		return NULL;
	}

	if (fieldTicket.providerType == eOCT_DATAPATTERN_SINGLE) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context singles are not yet implemented\n");
		return NULL;
	}

	return NULL;
}

void* eOCT_field_read(eOCT_fieldTicket fieldTicket, OCT_index entryIndex, OCT_global contextHandle) {
	eOCT_pool* sourcePool;
	OCT_index actualIndex;
	if (fieldTicket.global && fieldTicket.globalPool) {
		sourcePool = fieldTicket.globalPool;
	}
	else {
		sourcePool = eOCT_field_getSourcePool(contextHandle, fieldTicket);
	}

	if (fieldTicket.providerType == eOCT_DATAPATTERN_SINGLE) {
		actualIndex = fieldTicket.providerTypeIndex;
	}
	else if (entryIndex == OCT_INDEX_NULL) {
		OCT_ERROR_LOG(OCT_EXIT_SOURCE_MISMATCH, "Provided no entry index for a field from a non-SINGLE source");
		return NULL;
	}
	else {
		actualIndex = entryIndex;
	}
	void* fieldLoc = eOCT_pool_access(sourcePool, actualIndex, fieldTicket.offsetFromStruct);
	return fieldLoc;
}