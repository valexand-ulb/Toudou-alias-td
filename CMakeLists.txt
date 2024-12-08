cmake_minimum_required(VERSION 3.0)

project(todo C)
# Include the custom FindSQLite3.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Locate SQLite3
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${SQLITE3_INCLUDE_DIR})

add_executable(main main.c)

target_link_libraries(main ${SQLITE3_LIBRARY})

