cmake_minimum_required(VERSION 2.8)
project(ti_launchpad_playground)

include_directories(include)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
#set(HEADER_FILES
#        include/event.h
#        include/timer.h
#        include/types.h
#        include/watchdog.h)
#
#set(SOURCE_FILES
##        examples/blinkingLed.c
#        include/event.h
#        include/timer.h
#        include/types.h
#        include/watchdog.h
#        src/app/event.c
#        src/hal/timer.c
#        src/hal/watchdog.c
#        test/test_event.c
#        test/test_timer.c
#        test/test_watchdog.cpp)
set(SOURCE_FILES examples/blinkingLed.c)

add_executable(ti_launchpad_playground ${SOURCE_FILES})

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(ti_launchpad_playground ti_launchpad_library)