cmake_minimum_required(VERSION 3.28.1)
project(ste VERSION 1.0.0)
string(TIMESTAMP PROJECT_PUBLICATION_YEAR "%Y")

configure_file(config.hpp.in config.hpp @ONLY)
include_directories(${PROJECT_BINARY_DIR})


set(SOURCE_FILES
    src/main.cpp
    src/sources/ste.cpp
    src/sources/Editor.cpp
    src/sources/TextBuffer.cpp
    src/sources/FileHandler.cpp
)
set(FLAGS -Wall -Wextra)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE src/include)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Unknown")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("-- Build type: Debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("-- Build type: Release")
    target_link_options(${PROJECT_NAME} PRIVATE -static)
else()
    message("-- Build type: Unknown")
endif()