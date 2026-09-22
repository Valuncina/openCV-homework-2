#include <iostream>
#include <Eigen/Dense>

int main() {
    Eigen::Matrix2d mat;
    mat << 1,2,3,4;
    std::cout << "Matrix:\n" << mat << std::endl;
    Eigen::Vector2d vec(4,3);
    Eigen::Vector2d result = mat * vec;
    std::cout << "Result:\n" << result << std::endl;
    return 0;



}