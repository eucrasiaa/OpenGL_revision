cmake_minimum_required(VERSION 3.20)
project(OpenGL_Revision LANGUAGES C CXX)
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Do not build in-source! Use: cmake -B build && cmake --build build")
endif()

# project configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# options
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
option(BUILD_TESTS "Build unit tests" OFF)
# Backend Selection
set(BACKEND "SDL2" CACHE STRING "Graphics backend (SDL2 or RAYLIB)")
set_property(CACHE BACKEND PROPERTY STRINGS "SDL2" "RAYLIB")

# source file setup
set(PROJECT_SOURCES
    src/main.cpp
    src/core/Engine/Engine.cpp
    src/core/RenderServer/RenderServer.cpp
    src/nodeSystem/Node.cpp
    src/core/Managers/InputManager/CoreInputManager.cpp
    src/core/Managers/SceneManager/SceneManager.cpp

    src/core/InterpolateServer/InterpolateServer.cpp
    src/core/WindowServer/OpenGLWindowServer.cpp
    src/nodeSystem/SpacialNode.cpp
    src/nodeSystem/TestingNode.cpp
    src/core/Managers/MeshManager/MeshManager.cpp 

    src/core/Managers/AssetManager/AssetManager.cpp 
    src/core/Managers/MaterialManager/MaterialManager.cpp 
    src/core/Managers/TextureManager/BindlessTextureManager.cpp 
    src/core/RenderScene/RenderScene.cpp
    # src/core/RenderServer/WindowServe/OpenGLWindowServer.cpp
)

# file(GLOB_RECURSE PROJECT_SOURCES
#     "src/*.cpp"
# )
#
file(GLOB_RECURSE PROJECT_HEADERS 
    "include/*.h" "include/*.hpp"
    "src/*.h" "src/*.hpp" 
    "third_party/tiny_obj_loader.h"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# include directs
target_include_directories(${PROJECT_NAME} PRIVATE
    include/w_math/types
    include/w_math/
    include/w_utils/
    include/
    src
    src/core
    src/nodeSystem
    src/core/Managers
    src/core/InterpolateServer
    src/core/Managers/InputManager/InputAid
    src/core/Managers/SceneManager
    ${CMAKE_CURRENT_SOURCE_DIR} #for the rest 
)
target_precompile_headers(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/debug_utils/DBG_DEFS.h"
)


# Compiler Configuration
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/Od;/Zi;/D_DEBUG>
        $<$<CONFIG:Release>:/O2;/DNDEBUG>
    )
else()
    # GCC/Clang
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g3;-O0;-DDEBUG>
        $<$<CONFIG:Release>:-O3;-DNDEBUG;-march=native>
    )
    
    # Linker flags
    if(MINGW)
        target_link_options(${PROJECT_NAME} PRIVATE 
            "-static-libgcc" "-static-libstdc++" "-static"
            "-fuse-ld=lld"
        )
    endif()
endif()

if(ENABLE_ASAN OR ENABLE_UBSAN)
    if(MSVC)
        if(ENABLE_ASAN)
            target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address)
        endif()
    else()
        set(SANITIZER_FLAGS "")
        if(ENABLE_ASAN)
            set(SANITIZER_FLAGS "${SANITIZER_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        endif()
        if(ENABLE_UBSAN)
            set(SANITIZER_FLAGS "${SANITIZER_FLAGS} -fsanitize=undefined")
        endif()
        if(SANITIZER_FLAGS)
            target_compile_options(${PROJECT_NAME} PRIVATE ${SANITIZER_FLAGS})
            target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZER_FLAGS})
        endif()
    endif()
endif()


# Coverage Support
if(ENABLE_COVERAGE AND NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_options(${PROJECT_NAME} PRIVATE --coverage)
endif()

#locate deps

# OpenGL
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# Glad - local 
add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include")
target_link_libraries(glad PUBLIC OpenGL::GL Threads::Threads ${CMAKE_DL_LIBS})
add_library(glad::glad ALIAS glad)

# Backend
if(BACKEND STREQUAL "SDL2")
    find_package(SDL2 REQUIRED CONFIG)
    if(NOT SDL2_FOUND AND MINGW AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SDL2_DIR "C:/msys64/clang64/lib/cmake/SDL2" CACHE PATH "Force Clang64 SDL2" FORCE)
        find_package(SDL2 REQUIRED CONFIG)
    endif()
    
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad SDL2::SDL2)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_BACKEND_SDL2)
    
elseif(BACKEND STREQUAL "RAYLIB")
    find_package(raylib REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad raylib::raylib)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_BACKEND_RAYLIB)
endif()

# Windows-specific libraries
if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        setupapi
        winmm
        version
        imm32
        mingw32
        SDL2main 
         -lstdc++exp
    )
endif()

# Post-Build Steps
# windows dll copy
if(WIN32 AND TARGET SDL2::SDL2)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

# customs
add_custom_target(run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)


#valgring
if(ENABLE_VALGRIND)
    find_program(VALGRIND_PROGRAM valgrind)
    if(VALGRIND_PROGRAM)
        add_custom_target(valgrind
            COMMAND ${VALGRIND_PROGRAM} --leak-check=full --track-origins=yes $<TARGET_FILE:${PROJECT_NAME}>
            DEPENDS ${PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running Valgrind memory analysis"
        )
    endif()
endif()

# ?
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "========================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  Backend:        ${BACKEND}")
message(STATUS "  C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "  ASan:           ${ENABLE_ASAN}")
message(STATUS "  UBSan:          ${ENABLE_UBSAN}")
message(STATUS "  Coverage:       ${ENABLE_COVERAGE}")
message(STATUS "  Tests:          ${BUILD_TESTS}")
message(STATUS "========================================")
