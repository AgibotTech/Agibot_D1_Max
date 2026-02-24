cmake_minimum_required(VERSION 3.8)
project(example LANGUAGES CXX)

find_package(Threads REQUIRED)

# -------------------------------
# 1. 设置 C++ 标准
# -------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# 2. 设置 SDK 根路径（自动推断）
# -------------------------------
# 假设当前 CMakeLists.txt 位于 install/example/
# 则 SDK 根目录为 install/
get_filename_component(SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

message(STATUS "Using SDK root: ${SDK_ROOT}")

# -------------------------------
# 3. 设置包含目录与库目录
# -------------------------------
include_directories(${SDK_ROOT}/include)
link_directories(${SDK_ROOT}/lib/${CMAKE_SYSTEM_PROCESSOR})

# -------------------------------
# 4. 添加可执行文件
# -------------------------------
add_executable(data data.cpp)
add_executable(control control.cpp)
add_executable(async async.cpp)
add_executable(sync sync.cpp)
add_executable(manual_reconnect manual_reconnect.cpp)
add_executable(auto_reconnect auto_reconnect.cpp)
add_executable(take_control take_control.cpp)

# -------------------------------
# 5. 链接 SDK 库
# -------------------------------
target_link_libraries(data PRIVATE robot_sdk)
target_link_libraries(control PRIVATE robot_sdk)
target_link_libraries(async PRIVATE robot_sdk Threads::Threads)
target_link_libraries(sync PRIVATE robot_sdk)
target_link_libraries(manual_reconnect PRIVATE robot_sdk Threads::Threads)
target_link_libraries(auto_reconnect PRIVATE robot_sdk)
target_link_libraries(take_control PRIVATE robot_sdk Threads::Threads)

# -------------------------------
# 6. 运行时库搜索路径 (RPATH)
# -------------------------------
# 确保运行时能找到 install/lib 下的库
set_target_properties(data PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(control PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(async PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(sync PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(manual_reconnect PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(auto_reconnect PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)
set_target_properties(take_control PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)