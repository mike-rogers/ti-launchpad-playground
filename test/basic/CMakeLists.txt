include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runBasicTests
        test_watchdog.cpp
        test_timer.cpp
        )

target_link_libraries(runBasicTests gtest gtest_main)
target_link_libraries(runBasicTests ti_launchpad_library)