set(BINARY ${CMAKE_PROJECT_NAME}_run)
set(LIBRARY ${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY})

add_library(${LIBRARY} STATIC ${SOURCES})
