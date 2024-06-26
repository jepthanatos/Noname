set(BINARY ${CMAKE_PROJECT_NAME}_tst)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} MainAllTests.cpp)

target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(${BINARY} ${CMAKE_PROJECT_NAME}_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${BINARY})
