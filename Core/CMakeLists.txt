
add_library(Core STATIC
        internal/layout/layout.c
        internal/layout/dataAccess.c
        engine/registry/dataAccess_Eng.h
        engine/data/types_eng.h

        internal/utilities/utilities_int.h
        internal/utilities/IDMap.c
        internal/utilities/mappedPool.c
        internal/utilities/pools.c

        internal/errors/errors.c
        engine/errors/types_eng.h
        engine/errors/errors_eng.h

        include/math/matrices/mat3.c
        include/math/units/constants.c
        include/math/units/conversions.c
        include/math/vectors/vec2.c
        include/math/shapes/rect2.c

        internal/ECS/entity.c
        internal/ECS/ECS.c
        internal/ECS/entityContext.c

        internal/registry/registry.c

        engine/resources/resources_eng.h
        internal/resources/images_int.h
        internal/resources/images.c
        internal/resources/types_int.h
        engine/resources/types_eng.h
        internal/resources/resources_int.h
        internal/resources/resources.c

        internal/scheduler/scheduler_int.h
        internal/scheduler/types_int.h
        internal/scheduler/scheduler.c

        internal/events/events_int.h
        internal/events/events.c
        internal/events/types_int.h

        include/hardware/buttons.h

        internal/globals/globals_int.h
        internal/globals/types_int.h
        internal/globals/globals.c
        engine/globals/globals_eng.h
        engine/globals/types_eng.h
        include/utilities/sets.h
        include/utilities/utilities.h
        include/math/lines/line2.h
        include/math/lines/line2.c
        include/utilities/sets.c
        include/utilities/config.h
        init/systemDescription.c
        internal/utilities/allocations_int.h
        internal/utilities/allocations.c
        include/utilities/memory.h
        init/Core_shutdown.h
        engine/registry/fields_eng.h
        engine/registry/dataTypes_eng.h
        engine/registry/dataPatterns_eng.h
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(Core PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(CoreInit INTERFACE)
target_include_directories(CoreInit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/init)

target_link_libraries(Core PRIVATE stb_image)