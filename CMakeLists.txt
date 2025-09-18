
cmake_minimum_required(VERSION 3.10)

project(app CXX)

set(app "app")

message("statrt to build ${app} ...")

# debug  release should remove this line
set(CMAKE_BUILD_TYPE Debug)

# for dll import use case 
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# 包含引擎头文件
include_directories(zidian/include)

# 引擎子目录
add_subdirectory(zidian)

# 应用源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
aux_source_directory(application APP_SRCS)

if (WIN32)
    message(STATUS "Using Win32 library")
endif()

add_executable(${PROJECT_NAME} ${APP_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE zidian)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wall")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")

# -------------------------
# 安装规则
# -------------------------

# 安装 zidian 引擎头文件
install(DIRECTORY zidian/include/ DESTINATION include)
# 安装 zidian 生成的库（dll/so/lib）
install(TARGETS zidian
        EXPORT zidianTargets
        LIBRARY DESTINATION lib       # for .so
        ARCHIVE DESTINATION lib       # for .a
        RUNTIME DESTINATION bin)      # for .dll

# 导出 cmake 配置，方便其他项目 find_package()
install(EXPORT zidianTargets
        FILE zidianConfig.cmake
        NAMESPACE zidian::
        DESTINATION lib/cmake/zidian)