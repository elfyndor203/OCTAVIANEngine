#include "OCT_Core_eng.h"
#include <stddef.h>

#include "systemEx/systemEx_int.h"
#include "componentEx/componentEx_int.h"

void system_register_systemEx() {

	// FOR EACH COMPONENT
		// Describe each field:
			eOCT_fieldDescription field1 = { "someField", eOCT_TYPE_INT64, offsetof(iOCT_componentEx, fieldA) };
			eOCT_fieldDescription field2 = { "someOtherField", eOCT_TYPE_INT64, offsetof(iOCT_componentEx, fieldB) };
		// Create a componentDescription
		eOCT_componentDescription componentEx = { "componentEx", sizeof(componentEx), eOCT_generateFieldDescriptionPool(2, field1, field2, eOCT_END_FIELDS)};
	//

	// FOR EACH SYSTEM
		// Describe each field request:
			eOCT_fieldRequest heightReq = { "height", eOCT_TYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest widthReq = { "width", eOCT_TYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest mightNeed = { "depth", eOCT_TYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest keys = {
				.name = "glfwKeys",
				.type = eOCT_TYPE_INT64,
				.optional = false,
				.ticketCache = &iOCT_systemEx_inst.keyCache,
				.providerType = eOCT_DATAPATTERN_EVENT
			};
			eOCT_fieldRequest keyPress = {
				.name = "glfwKeyPress",
				.type = eOCT_TYPE_BOOL8,
				.optional = false,
				.ticketCache = &iOCT_systemEx_inst.keyPressCache,
				.providerType = eOCT_DATAPATTERN_EVENT
			};
			eOCT_fieldRequest keyRelease = {
				.name = "glfwKeyRelease",
				.type = eOCT_TYPE_BOOL8,
				.optional = false,
				.ticketCache = &iOCT_systemEx_inst.keyReleaseCache,
				.providerType = eOCT_DATAPATTERN_EVENT
			};
	//

	eOCT_fieldDescription testSingleField = {
		.name = "testSingleField",
		.type = eOCT_TYPE_INT64,
		.providerType = eOCT_DATAPATTERN_SINGLE,
		.offset = 0
	};
	eOCT_singleDescription testSingle = {
		.name = "testGlobal",
		.providedField = testSingleField,
		.keyCacheLocation = NULL,
		.global = true
	};

	// ensure the correct counts
	eOCT_systemDescription templateSystem = {
		.name = "_SystemEx",
		.providedComponents = eOCT_generateComponentDescriptionPool(1, componentEx, eOCT_END_COMPONENTS),
		.requestedFields = eOCT_generateFieldRequestPool(6, heightReq, widthReq, mightNeed, keys, keyPress, keyRelease, eOCT_END_REQUESTS),
		.providedEvents = eOCT_POOL_EMPTY,
		.providedSingles = eOCT_generateSingleDescriptionPool(1, testSingle, eOCT_END_SINGLES),
		.providedDataPools = eOCT_POOL_EMPTY,
		.systemInitFx = system_init_systemEx
	};

	iOCT_systemEx_inst.systemID = eOCT_registry_registerSystem(templateSystem);
}