cmake_minimum_required(VERSION 3.28.1)
project(ste)

set(SOURCE_FILES
    src/main.cpp
    src/sources/ste.cpp
    src/sources/Editor.cpp
    src/sources/TextBuffer.cpp
    src/sources/FileHandler.cpp
)
set(FLAGS -Wall -Wextra)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


enable_testing()
add_test(NAME test COMMAND test)