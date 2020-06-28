cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(acidcamGL LANGUAGES CXX)
set(ACIDCUBE_MAJOR_VERSION 1)
set(ACIDCUBE_MINOR_VERSION 0)
set(ACIDCUBE_VERSION ${ACIDCAM_MINOR_VERSION}.${ACIDCAM_MAJOR_VERSION})
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build configuration is: ${CMAKE_BUILD_TYPE}")
message(STATUS "is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")

if(CMAKE_CXX_COMPILER_LOADED)
	message(STATUS "The C++ Compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
	message(STATUS "The C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQURIED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
list(APPEND COMPILER_FLAGS "-Ofast")
else()
message(STATUS "Debug Mode")
list(APPEND COMPILER_FLAGS "-g")
endif()


find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}  ${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})
link_directories(/usr/local/lib /usr/lib)

add_executable(acidcamGL ${CMAKE_SOURCE_DIR}/source/main.cpp ${CMAKE_SOURCE_DIR}/source/gl_shader.cpp ${CMAKE_SOURCE_DIR}/source/gl_window.cpp)
target_link_libraries(acidcamGL ${OPENGL_gl_LIBRARY} ${GLEW_SHARED_LIBRARIES} GLEW Threads::Threads glfw ${GLEW_LIBARIES} ${OpenCV_LIBS} acidcam)
target_compile_options(acidcamGL PRIVATE ${COMPILER_FLAGS})

