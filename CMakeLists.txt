
cmake_minimum_required(VERSION 3.10)

project(app CXX)

set(app "app")

message("statrt to build ${app} ...")

# debug  release should remove this line
set(CMAKE_BUILD_TYPE Debug)

# for dll import use case 
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})  
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_FLAGS "-std=c++17")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "This Linux platform")
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl -lX11 -latomic")
    set (CMAKE_CXX_FLAGS -pthread)
elseif(APPLE)
    message(STATUS "This Macos platform")
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL -framework CoreFoundation -framework CoreAudio -framework AudioToolbox")
elseif (WIN32)
    message(STATUS "This Windows platform")
endif()


include_directories(zidian/include)
# include_directories(vender/glfw/include)

add_subdirectory(zidian)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
aux_source_directory(application SRCS)

if (WIN32)
    message(STATUS "Using Win32 library")
    # link_directories(lib/win64)
endif()

add_executable(${PROJECT_NAME} ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")

target_link_libraries(${PROJECT_NAME} zidian)

