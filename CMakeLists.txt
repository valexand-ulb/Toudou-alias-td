cmake_minimum_required(VERSION 3.0)

project(todo C)
# Include the custom FindSQLite3.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Locate SQLite3
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${SQLITE3_INCLUDE_DIR})

add_executable(td main.c
        database.h
        database.c
        log.h
        utils.c
        utils.h
        constant.h)

target_link_libraries(td ${SQLITE3_LIBRARY})

