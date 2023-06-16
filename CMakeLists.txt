cmake_minimum_required(VERSION 3.24)

project(KeyKG_Code)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES 
    src/bc.cpp
    src/main.cpp
    src/file_format.cpp
    src/double.cpp
    src/timestamp.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wconversion -Wshadow -O2")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include    
)
