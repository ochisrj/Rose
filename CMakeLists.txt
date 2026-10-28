cmake_minimum_required(VERSION 3.15)
project(Rose LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

# --- GLFW: try system, else FetchContent, else prebuilt Windows lib ---
include(FetchContent)
find_package(glfw3 QUIET)

if(NOT glfw3_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Rose/vendor/glfw/CMakeLists.txt")
        add_subdirectory(Rose/vendor/glfw)
        set(GLFW_TARGET glfw)
    elseif(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Rose/vendor/glfw/lib-vc2022/glfw3.lib")
        add_library(glfw UNKNOWN IMPORTED)
        set_target_properties(glfw PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Rose/vendor/glfw/lib-vc2022/glfw3.lib"
        )
        set(GLFW_TARGET glfw)
        message(STATUS "Using prebuilt Rose/vendor/glfw/lib-vc2022/glfw3.lib")
    else()
        message(STATUS "Fetching GLFW 3.4 (no system glfw3 found)")
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.4
        )
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw)
        set(GLFW_TARGET glfw)
    endif()
else()
    set(GLFW_TARGET glfw)
endif()

add_executable(Rose
    Rose/src/main.cpp
    Rose/render/camera.cpp
    Rose/render/stb.cpp
    Rose/shaders/shaderclass.cpp
    Rose/shaders/VAO.cpp
    Rose/shaders/VBO.cpp
    Rose/shaders/EBO.cpp
    Rose/vendor/glad/src/glad.c
    Rose/vendor/imgui/imgui.cpp
    Rose/vendor/imgui/imgui_draw.cpp
    Rose/vendor/imgui/imgui_tables.cpp
    Rose/vendor/imgui/imgui_widgets.cpp
    Rose/vendor/imgui/imgui_demo.cpp
    Rose/vendor/imgui/backends/imgui_impl_glfw.cpp
    Rose/vendor/imgui/backends/imgui_impl_opengl3.cpp
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Rose/pc_spec.cpp")
    target_sources(Rose PRIVATE Rose/pc_spec.cpp)
endif()

target_include_directories(Rose PRIVATE
    Rose/shaders
    Rose/render
    Rose/src
    Rose/vendor
    Rose/vendor/glad/include
    Rose/vendor/glfw/include
    Rose/vendor/glm
    Rose/vendor/stb
    Rose/vendor/imgui
    Rose/vendor/imgui/backends
)

target_link_libraries(Rose PRIVATE
    OpenGL::GL
    ${GLFW_TARGET}
)

if(WIN32)
    target_link_libraries(Rose PRIVATE opengl32)
elseif(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
    target_link_libraries(Rose PRIVATE ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK} ${COREVIDEO_FRAMEWORK})
else()
    find_package(Threads REQUIRED)
    find_package(X11 QUIET)
    target_link_libraries(Rose PRIVATE Threads::Threads dl m)
    if(X11_FOUND)
        target_link_libraries(Rose PRIVATE ${X11_LIBRARIES})
    endif()
endif()

if(MSVC)
    target_compile_options(Rose PRIVATE /W3)
else()
    target_compile_options(Rose PRIVATE -Wall -Wextra)
endif()

# Copy shaders to build dir so "./shaders/shader.vert" works regardless of build folder
add_custom_command(TARGET Rose POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Rose/shaders"
        "$<TARGET_FILE_DIR:Rose>/shaders"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:Rose>/shaders"
)
# Also copy to flat layout for legacy path
add_custom_command(TARGET Rose POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Rose/shaders/shader.vert"
        "$<TARGET_FILE_DIR:Rose>/shader.vert"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Rose/shaders/shader.frag"
        "$<TARGET_FILE_DIR:Rose>/shader.frag"
)

set_target_properties(Rose PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
