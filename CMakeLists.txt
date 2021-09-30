cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(Game2D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wextra")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

set(FILE "${CMAKE_SOURCE_DIR}/Dependencies/glew/include/GL/glew.h")
message(STATUS "Checking if glew is build")
if (NOT EXISTS "${FILE}")
    if(UNIX)
        message(STATUS "Building Glew")
        execute_process(COMMAND make -C ${CMAKE_SOURCE_DIR}/Dependencies/glew/auto)
    else()
        message(FATAL_ERROR "Please build GLEW first:\n"
                "1. Open Git Bash in Dependencies/glew/auto\n"
                "2. Execute mingw-32-make.exe\n"
                "3. Reload the Cmake Project")
    endif()
endif()

find_package(Python REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCES
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/Block.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/BlockAnimator.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/DisplayBlock.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/Enemy.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/EnemyAnimator.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Controller/Game.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/GameInformation.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/GameLevel.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/GameObject.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/PengoAnimator.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/Player.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/ResourceManager.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/Shader.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/SpriteRenderer.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/Texture.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/Wall.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/WallAnimator.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/Model/Score.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/util.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/SoundManager.cpp
        ${CMAKE_SOURCE_DIR}/Game2D/src/View/SoundModule.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/glew/build/cmake)
#add_subdirectory(backward-cpp)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/libsndfile)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/openal-soft)
add_subdirectory(${CMAKE_SOURCE_DIR}/Tests)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/libsndfile/include)
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/openal-soft/include/AL)
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/glew/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Game2D/src/View/Application.cpp ${SOURCES})
#target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
#target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2019/Community/VC/Tools/Llvm/x64/lib/clang/11.0.0/lib/windows")
#target_link_libraries(${PROJECT_NAME} PRIVATE clang_rt.asan_dynamic-x86_64 clang_rt.asan_dynamic_runtime_thunk-x86_64)
#target_link_options(${PROJECT_NAME} PRIVATE /wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} glew_s glfw OpenAL sndfile)
