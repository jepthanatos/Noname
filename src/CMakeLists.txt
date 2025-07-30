set(BINARY ${CMAKE_PROJECT_NAME}_run)
set(LIBRARY ${CMAKE_PROJECT_NAME}_lib)

# Explicitly list source files
set(SOURCES
    Character.cpp
    Container.cpp
    FileManager.cpp
    GameManager.cpp
    Inventory.cpp
    LogManager.cpp
    Noname.cpp
    ItemTemplate.cpp
    ItemTemplateFactory.cpp
)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY})

add_library(${LIBRARY} STATIC ${SOURCES})
