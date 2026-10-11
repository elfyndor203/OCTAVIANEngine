add_library(Renderer STATIC
        system/systemDescription.c
        internal/renderer/renderer.c
        internal/renderer/texture/textureGroup.c
        internal/renderer/sprite2D/sprite2D.c
        internal/renderer/shader/shader.c
        include/renderer/sprite2D.h

        internal/window/window/window.c
        internal/window/window/callbacks.c
        internal/window/windowSystem.c
        internal/window/types_int.h
        internal/renderer/camera2D/camera2D_int.h
        internal/renderer/camera2D/camera2D.c
        include/renderer/camera2D.h
        internal/window/inputs/inputs.c
        include/renderer/colors.h
        include/window/inputs.h
)

target_compile_definitions(Renderer PRIVATE
        iOCT_RENDERER_DEFAULT_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/internal/renderer/shader"
)

target_include_directories(Renderer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/system
)

set_target_properties(Renderer PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

target_link_libraries(Renderer PUBLIC Core PRIVATE glad glfw)

add_library(RendererSystem INTERFACE)
target_include_directories(RendererSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)