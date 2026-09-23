# 1. 声明 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 声明项目名称和使用的语言
project(opencv_1 CXX)

# 3. 设置 C++ 标准（推荐 C++17 或 14）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 查找依赖库
# 查找 OpenCV
find_package(OpenCV REQUIRED)
# 查找 Eigen3（Eigen 通常提供 Eigen3::Eigen 这个目标，如果报错可以改成 find_package(Eigen3 REQUIRED)）
find_package(Eigen3 REQUIRED)

# 5. 添加头文件搜索路径（现代 CMake 其实更推荐 target_include_directories，为了新手好懂先这么写）
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(main main.cpp func.cpp)
target_link_libraries(main ${OpenCV_LIBS})
