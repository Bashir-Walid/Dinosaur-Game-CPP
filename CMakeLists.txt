cmake_minimum_required(VERSION 3.15)
project(RaylibGame)

set(CMAKE_CXX_STANDARD 17)

# ربط vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(raylib CONFIG REQUIRED)

add_executable(MyGame main.cpp)
target_link_libraries(MyGame PRIVATE raylib)
