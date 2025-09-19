cmake_minimum_required(VERSION 3.10)

project(zidian CXX)

set(CMAKE_BUILD_TYPE Debug)

include_directories(vender/glfw/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS inlcude/*.h)

if (WIN32)
    message(STATUS "Using Windows library")
    # set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/windows/libglfw3.a)
    # link_directories(${CMAKE_SOURCE_DIR}/zidian/lib/windows)
    # set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/windows/libglfw3.a)

    add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/zidian/lib/windows/libglfw3.a
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/zidian/vender/glfw/include
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Using Linux library")
    # link_directories(${CMAKE_SOURCE_DIR}/lib/linux)

    add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/zidian/lib/linux/libglfw3.a
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/zidian/vender/glfw/include
    )
else()
    message(STATUS "Unknow platform")
endif()

# 动态库
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 静态库
# add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")

target_link_libraries(${PROJECT_NAME} glfw3)

# add_custom_command(TARGET zidian POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/assets
#     ${CMAKE_BINARY_DIR}/assets
# )