# FindSQLite3.cmake
# Locate SQLite3 library and headers

find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY NAMES sqlite3)

if(SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
    set(SQLITE3_FOUND TRUE)
else()
    set(SQLITE3_FOUND FALSE)
endif()

if(SQLITE3_FOUND)
    message(STATUS "Found SQLite3: ${SQLITE3_LIBRARY}")
else()
    message(FATAL_ERROR "SQLite3 not found!")
endif()

mark_as_advanced(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARY)
