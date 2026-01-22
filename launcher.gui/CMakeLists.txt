cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(acidcamGL_launcher LANGUAGES CXX C)
set(LAUNCHER_MAJOR_VERSION 1)
set(LAUNCHER_MINOR_VERSION 3)
set(LAUNCHER_PATCH_VERSION 0)
set(LAUNCHER_VERSION ${LAUNCHER_MAJOR_VERSION}.${LAUNCHER_MINOR_VERSION}.${LAUNCHER_PATCH_VERSION})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "============================================")
message(STATUS "Building ${PROJECT_NAME} v${LAUNCHER_VERSION}")
message(STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")
message(STATUS "============================================")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND COMPILER_FLAGS "-O3")
else()
    message(STATUS "Debug Mode enabled")
    list(APPEND COMPILER_FLAGS "-g")
endif()

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/local/lib;/usr/lib")

message(STATUS "")
message(STATUS "--- Checking Dependencies ---")

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network)
if(Qt6_FOUND)
    message(STATUS "Qt6 Found")
    message(STATUS "  Qt6Core: ${Qt6Core_LIBRARIES}")
    message(STATUS "  Qt6Gui: ${Qt6Gui_LIBRARIES}")
    message(STATUS "  Qt6Widgets: ${Qt6Widgets_LIBRARIES}")
    message(STATUS "  Qt6Concurrent: ${Qt6Concurrent_LIBRARIES}")
    message(STATUS "  Qt6Network: ${Qt6Network_LIBRARIES}")
else()
    message(FATAL_ERROR "Qt6 not found! Please install Qt6 development packages.")
endif()

# Find Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS "Threads library found")
else()
    message(FATAL_ERROR "Threads library not found!")
endif()

message(STATUS "")
message(STATUS "--- Dependency Check Complete ---")
message(STATUS "")

# Launcher GUI sources
set(LAUNCHER_GUI_HEADERS
    main_window.hpp
    options_window.hpp
    auto_window.hpp
    sock_server.hpp
)

set(LAUNCHER_GUI_SOURCES
    main.cpp
    main_window.cpp
    options_window.cpp
    auto_window.cpp
    sock_server.cpp
)

set(LAUNCHER_GUI_RESOURCES
    qresource.qrc
)

add_executable(acidcamGL_launcher
    ${LAUNCHER_GUI_HEADERS}
    ${LAUNCHER_GUI_SOURCES}
    ${LAUNCHER_GUI_RESOURCES}
)

target_include_directories(acidcamGL_launcher
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Gui_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6Concurrent_INCLUDE_DIRS}
        ${Qt6Network_INCLUDE_DIRS}
)

target_link_libraries(acidcamGL_launcher
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Concurrent
        Qt6::Network
        Threads::Threads
)

target_compile_options(acidcamGL_launcher PRIVATE ${COMPILER_FLAGS})
install(TARGETS acidcamGL_launcher DESTINATION bin RENAME launcher)
message(STATUS "")
message(STATUS "============================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${LAUNCHER_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Target: acidcamGL_launcher")
message(STATUS "============================================")
