cmake_minimum_required(VERSION 3.8)
project(example LANGUAGES CXX)

find_package(Threads REQUIRED)

# -------------------------------
# 1. Set C++ Standard
# -------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# 2. Set SDK Root Path (Auto-Detected)
# -------------------------------
# Assume current CMakeLists.txt is located in install/example/
# Then the SDK root directory is install/
get_filename_component(SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

message(STATUS "Using SDK root: ${SDK_ROOT}")

# -------------------------------
# 3. Set Include and Library Directories
# -------------------------------
include_directories(${SDK_ROOT}/include)
link_directories(${SDK_ROOT}/lib/${CMAKE_SYSTEM_PROCESSOR})

# -------------------------------
# 4. Add Executable Files
# -------------------------------
add_executable(data data.cpp)
add_executable(control control.cpp)
add_executable(async async.cpp)
add_executable(sync sync.cpp)
add_executable(manual_reconnect manual_reconnect.cpp)
add_executable(auto_reconnect auto_reconnect.cpp)
add_executable(take_control take_control.cpp)
add_executable(camera_bitrate camera_bitrate.cpp)

# -------------------------------
# 5. Link SDK Library
# -------------------------------
target_link_libraries(data PRIVATE robot_sdk)
target_link_libraries(control PRIVATE robot_sdk)
target_link_libraries(async PRIVATE robot_sdk Threads::Threads)
target_link_libraries(sync PRIVATE robot_sdk)
target_link_libraries(manual_reconnect PRIVATE robot_sdk Threads::Threads)
target_link_libraries(auto_reconnect PRIVATE robot_sdk)
target_link_libraries(take_control PRIVATE robot_sdk Threads::Threads)
target_link_libraries(camera_bitrate PRIVATE robot_sdk)
# -------------------------------
# 6. Runtime Library Search Path (RPATH)
# -------------------------------
# Ensure that the runtime can find libraries in install/lib
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
set_target_properties(camera_bitrate PROPERTIES
    BUILD_RPATH "${SDK_ROOT}/lib/${ARCH_DIR}"
)