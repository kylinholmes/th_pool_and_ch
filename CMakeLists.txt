## Project Info
cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_CMAKE} CACHE STRING "Vcpkg toolchain file")
project(th_pool VERSION 0.0.1 LANGUAGES C CXX)


## Compile Config
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
if( NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message("Setting default build type to Debug")
endif()


## Include dir
include_directories(
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
file(GLOB_RECURSE SOURCE "src/*.cpp")


##  Exec and Link
add_executable(${PROJECT_NAME} ${SOURCE})
find_package(fmt CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

## Dependencies
