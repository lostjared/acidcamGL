cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(acidcamGL LANGUAGES CXX C)

set(ACIDCAMGL_MAJOR_VERSION 1)
set(ACIDCAMGL_MINOR_VERSION 0)
set(ACIDCAMGL_VERSION ${ACIDCAMGL_MAJOR_VERSION}.${ACIDCAMGL_MINOR_VERSION})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "============================================")
message(STATUS "Building ${PROJECT_NAME} v${ACIDCAMGL_VERSION}")
message(STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")
message(STATUS "============================================")

if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
endif()

if(CMAKE_C_COMPILER_LOADED)
    message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND COMPILER_FLAGS "-O3")
else()
    message(STATUS "Debug Mode enabled")
    list(APPEND COMPILER_FLAGS "-g")
endif()

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/local/lib;/usr/lib")

include(CheckIncludeFileCXX)
include(CheckIncludeFile)
include(CheckLibraryExists)

message(STATUS "")
message(STATUS "--- Checking Dependencies ---")

set(OpenCV_FOUND FALSE)

message(STATUS "Looking for OpenCV in /usr first...")
set(OpenCV_DIR_HINTS
    /usr/lib/cmake/opencv4
    /usr/lib/cmake/opencv
    /usr/share/opencv4
    /usr/share/opencv
    /usr/lib64/cmake/opencv4
    /usr/lib64/cmake/opencv
)

foreach(hint ${OpenCV_DIR_HINTS})
    if(EXISTS "${hint}" AND NOT OpenCV_FOUND)
        set(OpenCV_DIR "${hint}")
        find_package(OpenCV QUIET CONFIG)
        if(OpenCV_FOUND)
            message(STATUS "Found OpenCV ${OpenCV_VERSION} in /usr: ${OpenCV_DIR}")
            break()
        endif()
    endif()
endforeach()

if(NOT OpenCV_FOUND)
    message(STATUS "OpenCV not found in /usr, checking other locations...")
    set(OpenCV_DIR_FALLBACK
        /usr/local/lib/cmake/opencv4
        /usr/local/share/opencv4
        /opt/opencv/lib/cmake/opencv4
        $ENV{HOME}/opencv/lib/cmake/opencv4
    )
    
    foreach(hint ${OpenCV_DIR_FALLBACK})
        if(EXISTS "${hint}" AND NOT OpenCV_FOUND)
            set(OpenCV_DIR "${hint}")
            find_package(OpenCV QUIET CONFIG)
            if(OpenCV_FOUND)
                message(STATUS "Found OpenCV ${OpenCV_VERSION} in: ${OpenCV_DIR}")
                break()
            endif()
        endif()
    endforeach()
endif()

if(NOT OpenCV_FOUND)
    unset(OpenCV_DIR CACHE)
    find_package(OpenCV REQUIRED)
endif()

if(OpenCV_FOUND)
    message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
    message(STATUS "OpenCV Include Dirs: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV Libraries: ${OpenCV_LIBS}")
else()
    message(FATAL_ERROR "OpenCV not found! Please install OpenCV development packages.")
endif()

check_include_file_cxx("opencv2/opencv.hpp" HAVE_OPENCV_HPP)
if(NOT HAVE_OPENCV_HPP)
    set(CMAKE_REQUIRED_INCLUDES ${OpenCV_INCLUDE_DIRS})
    check_include_file_cxx("opencv2/opencv.hpp" HAVE_OPENCV_HPP_WITH_PATH)
    if(NOT HAVE_OPENCV_HPP_WITH_PATH)
        message(WARNING "opencv2/opencv.hpp header not found directly, but OpenCV package was found.")
    endif()
endif()

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message(STATUS "OpenGL Found")
    message(STATUS "OpenGL Include Dir: ${OPENGL_INCLUDE_DIR}")
    message(STATUS "OpenGL Libraries: ${OPENGL_LIBRARIES}")
    message(STATUS "OpenGL Version: ${OPENGL_VERSION}")
    
    if(TARGET OpenGL::GL)
        message(STATUS "Using modern OpenGL::GL target")
    endif()
else()
    message(FATAL_ERROR "OpenGL not found! Please install OpenGL development packages.")
endif()

check_include_file("GL/glew.h" HAVE_GLEW_H)
check_include_file("GL/gl.h" HAVE_GL_H)
if(NOT HAVE_GL_H)
    check_include_file("OpenGL/gl.h" HAVE_OPENGL_GL_H)  # macOS
    if(NOT HAVE_OPENGL_GL_H)
        message(WARNING "OpenGL header (GL/gl.h) not found directly.")
    endif()
endif()

find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS "GLFW3 Found")
else()
    message(FATAL_ERROR "GLFW3 not found! Please install GLFW3 development packages.")
endif()

check_include_file("GLFW/glfw3.h" HAVE_GLFW3_H)
if(NOT HAVE_GLFW3_H)
    set(CMAKE_REQUIRED_INCLUDES "/usr/include;/usr/local/include")
    check_include_file("GLFW/glfw3.h" HAVE_GLFW3_H_ALT)
endif()

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
    message(STATUS "PkgConfig Found: ${PKG_CONFIG_EXECUTABLE}")
else()
    message(FATAL_ERROR "PkgConfig not found!")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS "Threads library found")
else()
    message(FATAL_ERROR "Threads library not found!")
endif()

check_include_file("pthread.h" HAVE_PTHREAD_H)
if(NOT HAVE_PTHREAD_H)
    message(WARNING "pthread.h header not found!")
endif()

find_package(acidcam QUIET PATHS /usr/local)
if(NOT acidcam_FOUND)
    pkg_check_modules(acidcam QUIET acidcam)
endif()

if(acidcam_FOUND)
    message(STATUS "acidcam library found")
    message(STATUS "acidcam Include Dirs: ${acidcam_INCLUDE_DIRS}")
    message(STATUS "acidcam Library Dirs: ${acidcam_LIBRARY_DIRS}")
else()
    message(FATAL_ERROR "acidcam library not found! Please install libacidcam.")
endif()

set(GLM_INCLUDE_DIR "")
if(EXISTS "${CMAKE_SOURCE_DIR}/source/glm/glm.hpp")
    set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/source")
    message(STATUS "Using bundled GLM from: ${GLM_INCLUDE_DIR}")
else()
    check_include_file_cxx("glm/glm.hpp" HAVE_SYSTEM_GLM)
    if(HAVE_SYSTEM_GLM)
        message(STATUS "Using system GLM")
    else()
        foreach(glm_path "/usr/include" "/usr/local/include")
            if(EXISTS "${glm_path}/glm/glm.hpp")
                set(GLM_INCLUDE_DIR "${glm_path}")
                message(STATUS "Found GLM in: ${glm_path}")
                break()
            endif()
        endforeach()
        if(NOT GLM_INCLUDE_DIR AND NOT HAVE_SYSTEM_GLM)
            message(WARNING "GLM headers not found in system or source tree!")
        endif()
    endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/source/src/glad.c")
    message(FATAL_ERROR "GLAD source file not found at ${CMAKE_SOURCE_DIR}/source/src/glad.c")
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/source/include/glad/glad.h")
    message(WARNING "GLAD header might not be in expected location.")
endif()
message(STATUS "Using bundled GLAD library")

check_library_exists(dl dlopen "" HAVE_LIBDL)
if(HAVE_LIBDL)
    message(STATUS "Dynamic loading library (dl) found")
endif()

message(STATUS "")
message(STATUS "--- Dependency Check Complete ---")
message(STATUS "")


if(TARGET OpenGL::GL)
    set(GL_LIB OpenGL::GL)
    message(STATUS "Configured to use modern OpenGL::GL target")
else()
    set(GL_LIB ${OPENGL_gl_LIBRARY})
    message(STATUS "Warning: Using legacy OpenGL library (${OPENGL_gl_LIBRARY})")
endif()

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/source/src/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/source/include)

set(ACIDCAMGL_SOURCES
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/gl_shader.cpp
    ${CMAKE_SOURCE_DIR}/source/gl_window.cpp
    ${CMAKE_SOURCE_DIR}/source/keymap.cpp
    ${CMAKE_SOURCE_DIR}/source/acidcam_window.cpp
    ${CMAKE_SOURCE_DIR}/source/ipc_client.cpp
    ${CMAKE_SOURCE_DIR}/source/ffmpeg_write.cpp
    ${CMAKE_SOURCE_DIR}/source/plugin-program.cpp
    ${CMAKE_SOURCE_DIR}/source/stereo.cpp
)

set(ACIDCAMGL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/include
    ${OpenCV_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${acidcam_INCLUDE_DIRS}
    /usr/local/include
)

if(GLM_INCLUDE_DIR)
    list(APPEND ACIDCAMGL_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

set(ACIDCAMGL_LINK_DIRS
    ${acidcam_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
)

add_executable(acidcamGL ${ACIDCAMGL_SOURCES})
target_include_directories(acidcamGL PRIVATE ${ACIDCAMGL_INCLUDE_DIRS})
target_link_directories(acidcamGL PRIVATE ${ACIDCAMGL_LINK_DIRS})
target_link_libraries(acidcamGL 
    PRIVATE
        Threads::Threads
        glfw
        OpenGL::GL
        glad
        ${OpenCV_LIBS}
        acidcam
        ${CMAKE_DL_LIBS}
)

target_compile_options(acidcamGL PRIVATE ${COMPILER_FLAGS})
install(TARGETS acidcamGL DESTINATION bin)
message(STATUS "")
message(STATUS "============================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${ACIDCAMGL_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "============================================")
