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

# 6. 生成可执行文件 (左边是生成的名字，右边是源文件)
add_executable(opencv_1 opencv_1.cpp)

# 7. 链接库到可执行文件
target_link_libraries(opencv_1 ${OpenCV_LIBS})
target_link_libraries(opencv_1 Eigen3::Eigen)

# 生成可执行文件 (左边是生成的名字，右边是源文件)
add_executable(opencv_1_salt_pepper opencv_1_salt_pepper.cpp)

# 链接库到可执行文件
target_link_libraries(opencv_1_salt_pepper ${OpenCV_LIBS})
target_link_libraries(opencv_1_salt_pepper Eigen3::Eigen)

# 生成可执行文件 (左边是生成的名字，右边是源文件)
add_executable(opencv_1_sobel opencv_1_sobel.cpp)

# 链接库到可执行文件
target_link_libraries(opencv_1_sobel ${OpenCV_LIBS})
target_link_libraries(opencv_1_sobel Eigen3::Eigen)

add_executable(matrix_1 matrix_1.cpp)

target_link_libraries(matrix_1 Eigen3::Eigen)

add_executable(matrix_1_rotate matrix_1_rotate.cpp)

target_link_libraries(matrix_1_rotate Eigen3::Eigen)

add_executable(opencv_1_canny opencv_1_canny.cpp)

target_link_libraries(opencv_1_canny ${OpenCV_LIBS})
target_link_libraries(opencv_1_canny Eigen3::Eigen)