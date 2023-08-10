cmake_minimum_required(VERSION 3.12)
project(ctrl VERSION 1.0)

# find source files:
file(GLOB_RECURSE ctrl_src CONFIGURE_DEPENDS "src/*.c")
add_executable(${PROJECT_NAME} ${ctrl_src})

# find include diretories and libraries
find_package(GLFW3 3.3 REQUIRED)

include_directories("src/" ${GLFW3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glfw)