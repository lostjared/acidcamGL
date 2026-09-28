cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(acidcamGL LANGUAGES CXX C)

# --- 1. Project Metadata & Build Settings ---
set(ACIDCAMGL_VERSION 1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- 2. Build Options (Toggles) ---
option(ENABLE_MIDI "Build with RtMidi support" ON)
option(ENABLE_REACTIVE "Build with RtAudio support" ON)

# --- 3. Cross-Platform / Static Configuration ---
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()
if(WIN32)
    set(PLATFORM_LIBS winmm ksuser ole32 uuid setupapi)
    add_definitions(-DNO_SCREEN_GRAB)
endif()

# --- 4. Dependency Search ---
message(STATUS "--- Searching for Dependencies ---")

# Set OpenGL preference to use the modern GLVND (Linux)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)


# Standard Dependencies
if(MSVC)
    find_package(OpenCV REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(acidcam CONFIG REQUIRED)
    set(OPENCV_PC_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
    set(OPENCV_PC_LIBRARIES ${OpenCV_LIBS})
    set(GLFW_LIBRARIES glfw)
    set(ACIDCAM_LIBRARIES acidcam::acidcam)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(OPENCV_PC REQUIRED opencv5 opencv4)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(ACIDCAM REQUIRED acidcam)
endif()

# Optional Dependency: RtMidi
if(ENABLE_MIDI)
    if(MSVC)
        find_package(rtmidi CONFIG REQUIRED)
        set(RTMIDI_FOUND TRUE)
        set(RTMIDI_LIBRARIES RtMidi::rtmidi)
        find_path(RTMIDI_INCLUDE_DIRS RtMidi.h PATH_SUFFIXES rtmidi REQUIRED)
    else()
        pkg_check_modules(RTMIDI QUIET rtmidi)
    endif()
    if(RTMIDI_FOUND)
        add_definitions(-DMIDI_ENABLED)
        message(STATUS "RtMidi: Found (Enabled)")
    else()
        message(FATAL_ERROR "RtMidi not found. Disable ENABLE_MIDI or install dependency.")
    endif()
endif()

# Optional Dependency: RtAudio
if(ENABLE_REACTIVE)
    if(MSVC)
        find_package(RtAudio CONFIG REQUIRED)
        set(RTAUDIO_FOUND TRUE)
        set(RTAUDIO_LIBRARIES RtAudio::rtaudio)
    else()
        pkg_check_modules(RTAUDIO QUIET rtaudio)
    endif()
    if(RTAUDIO_FOUND)
        add_definitions(-DREACTIVE_ENABLED)
        message(STATUS "RtAudio: Found (Enabled)")
    else()
        message(FATAL_ERROR "RtAudio not found. Disable ENABLE_REACTIVE or install dependency.")
    endif()
endif()

# --- 5. Internal Libraries (GLAD) ---
add_library(glad STATIC source/src/glad.c)
target_include_directories(glad PRIVATE ./source/include)

# --- 6. Source Definitions ---
add_executable(acidcamGL source/acidcam_window.cpp
source/ffmpeg_write.cpp
source/gl_shader.cpp
source/gl_window.cpp
source/ipc_client.cpp
source/keymap.cpp
source/main.cpp
source/plugin-program.cpp
source/stereo.cpp
)

if(WIN32)
    set_target_properties(acidcamGL PROPERTIES WIN32_EXECUTABLE TRUE)
    if(MSVC)
        # Use the existing main() entry point with the Windows GUI subsystem.
        set_property(TARGET acidcamGL APPEND_STRING PROPERTY LINK_FLAGS " /ENTRY:mainCRTStartup")
    endif()
endif()

if(MSVC)
    target_sources(acidcamGL PRIVATE source/acidcamGL-Visual.Studio/getopt.cpp)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON) 
set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(MSVC)
    target_compile_options(acidcamGL PRIVATE /EHsc)
    target_compile_definitions(acidcamGL PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
else()
    target_compile_options(acidcamGL PRIVATE -O2)
endif()
add_definitions(-DNO_SCREEN_GRAB)

target_include_directories(acidcamGL PRIVATE 
    ./include
    ./source/include
    ${OPENCV_PC_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${ACIDCAM_INCLUDE_DIRS}
    ${RTMIDI_INCLUDE_DIRS}
    ${RTAUDIO_INCLUDE_DIRS}
)

# Linking order: Application -> Libraries -> System
# Using OpenGL::GL for the modern target-based approach
target_link_libraries(acidcamGL PRIVATE
    glad
    Threads::Threads
    OpenGL::GL
    ${OPENCV_PC_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${ACIDCAM_LIBRARIES}
    ${RTMIDI_LIBRARIES}
    ${RTAUDIO_LIBRARIES}
    ${PLATFORM_LIBS}
)

# --- 8. Installation ---
install(TARGETS acidcamGL DESTINATION bin)

# --- 9. Summary Report ---
message(STATUS "============================================")
message(STATUS " Configuration Summary for ${PROJECT_NAME}")
message(STATUS " Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS " MIDI Support: ${ENABLE_MIDI}")
message(STATUS " Reactive Support: ${ENABLE_REACTIVE}")
message(STATUS " Executable Name: acidcamGL")
message(STATUS "============================================")
message(STATUS "run ")
message(STATUS "sudo ldconfig /usr/local/lib")
