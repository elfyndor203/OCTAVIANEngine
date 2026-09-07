#include "Core_init.h"
#include "Core_shutdown.h"
#include "../system/systems_build.h"

// void OCT_engine_start(char* name, unsigned int windowX, unsigned int windowY, OCT_vec4 BGColor, unsigned int virtualX, unsigned int virtualY, unsigned int maxFPS, unsigned int ECSHz, unsigned int physicsHz) {
// 	OCT_WDWModule_init(name, windowX, windowY, BGColor);
// 	OCT_RESModule_init();
// 	OCT_RENModule_init((OCT_vec2) { virtualX, virtualY});
// 	OCT_ECSModule_init();
// 	OCT_PHYModule_init((OCT_vec2) { 0, OCT_GRAVITY_DEFAULT }, physicsHz);
// 	OCT_PLTModule_init();
//
// 	iOCT_OCTModule_init(maxFPS, ECSHz, physicsHz);
// }

// void OCT_engine_stop() {
// 	OCT_ECSModule_free();	// shut down engine
// 	OCT_RENModule_free();
// 	OCT_RESModule_free();
// 	OCT_WDWModule_free();
// }

/*
 * Order:
 *      Init parts of core with no dependencies
 *      Register all systems
 *      Build core systems based on the registry
 *      Distribute fields to external systems
 *      Init all external systems
 */
void OCT_engine_init() {
    init_OCT_memoryManager_init();
    init_OCT_registry_init();
    init_OCT_ECS_init();
    init_OCT_resources_init();
    init_OCT_globals_init();

    init_system_register_CORE();
    init_OCT_registerAllSystems();

    init_OCT_ECS_build();
    init_OCT_globals_build();

    init_OCT_registry_distributeTickets();
    init_OCT_registry_summary();

    init_OCT_registry_initAllSystems();
    init_OCT_registry_cleanup();
}

void OCT_engine_shutdown() {
    // iOCT_registry_cleanup();
}