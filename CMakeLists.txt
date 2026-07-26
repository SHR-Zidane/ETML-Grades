cmake_minimum_required(VERSION 3.15)
project(ETML_Grades LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(programme
    main.c
    gui.c
    data.c
    report.c
    report.h
)

target_include_directories(programme PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(programme PRIVATE ${GTK_CFLAGS_OTHER})
target_link_directories(programme PRIVATE ${GTK_LIBRARY_DIRS})

target_link_libraries(programme PRIVATE ${GTK_LIBRARIES})
if(NOT WIN32)
    target_link_libraries(programme PRIVATE m)
endif()
