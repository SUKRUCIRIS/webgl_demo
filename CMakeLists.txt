cmake_minimum_required(VERSION 3.10)
include(CMakePrintHelpers)

project(webgl_demo C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/public)

set(CGLM_SHARED OFF CACHE BOOL "" FORCE)
set(CGLM_STATIC ON CACHE BOOL "" FORCE)
set(CGLM_USE_C99 ON CACHE BOOL "" FORCE)
set(CGLM_USE_TEST OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)

set(OVERRIDE_CXX_FLAGS ON CACHE BOOL "" FORCE)
set(OVERRIDE_C_FLAGS ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_subdirectory(third_party/cglm)
add_subdirectory(third_party/assimp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/cglm/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/opengl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/assimp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/third_party/assimp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype)

FILE(GLOB_RECURSE MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**")

cmake_print_variables(MY_SOURCES)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

add_executable(
webgl_demo
${MY_SOURCES}
)

set_target_properties(webgl_demo PROPERTIES LINK_FLAGS "-O3 -s USE_GLFW=3 -s USE_FREETYPE=1 -s USE_ZLIB=1 -s ALLOW_MEMORY_GROWTH=1 -s BINARYEN_ASYNC_COMPILATION=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s WASM=1 --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/template.html --embed-file ${CMAKE_CURRENT_SOURCE_DIR}/assets/@/")

target_link_libraries(webgl_demo cglm assimp)
