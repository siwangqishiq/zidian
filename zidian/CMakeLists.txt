cmake_minimum_required(VERSION 3.10)

project(zidian CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS inlcude/*.h)

# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 动态库
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 静态库
# add_library(${PROJECT_NAME} STATIC ${SOURCES})

# target_link_libraries(${PROJECT_NAME} glfw3)