cmake_minimum_required(VERSION 2.8)

project(ti_launchpad_library)

set(SOURCE_FILES
        hal/timer.c
        hal/watchdog.c
        )

add_library(ti_launchpad_library STATIC ${SOURCE_FILES} ${HEADER_FILES})