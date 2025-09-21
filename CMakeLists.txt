cmake_minimum_required(VERSION 3.14)
project(MyGTKApp C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

file(GLOB SRC_FILES src/*.c)

add_executable(drivers-manager ${SRC_FILES})

target_link_libraries(drivers-manager PRIVATE PkgConfig::GTK4)

install(PROGRAMS build/drivers-manager DESTINATION /usr/bin)
install(PROGRAMS scripts/detect-gpu.sh DESTINATION /lib/drivers-manager)