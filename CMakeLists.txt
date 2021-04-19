cmake_minimum_required(VERSION 3.17)
project(opersys2)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5Core REQUIRED)

set(SOURCE_FILES ffinder.cpp scanner.cpp scanner.h arg_parser.cpp arg_parser.h arg_property.h errors.h)

add_executable(opersys2 ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core)