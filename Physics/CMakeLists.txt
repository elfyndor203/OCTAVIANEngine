add_library(Physics STATIC
        system/systemDescription.c
        system/systemDescription_physics.h
        internal/physics2D/physics2D_int.h
        internal/physics2D/types_int.h
        internal/physicsSystem_int.h
        internal/physicsSystem.c
        internal/types_int.h
        internal/physics2D/physics2D.c
        include/physics2D/physics2D.h
        include/OCT_Physics.h
        internal/constraints/rope2D.c
        internal/constraints/constraints_int.h
        internal/constraints/types_int.h
        include/physics2D/constraints.h
        internal/constraints/hitbox2D.c
        internal/collider2D/collider2D.c
        internal/collider2D/collider2D_int.h
        internal/constraints/distance2D.c
        internal/constraints/distance2D_int.h
        include/collider2D/collider2D.h
        internal/collider2D/collider2D_int.c
)

target_include_directories(Physics
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/system
)

set_target_properties(Physics PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

target_link_libraries(Physics PUBLIC Core PRIVATE box2d)

add_library(PhysicsSystem INTERFACE)
target_include_directories(PhysicsSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

