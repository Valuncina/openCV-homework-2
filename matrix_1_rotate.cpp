#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>


int main() {
    Eigen::Matrix2d mat ;
    double x = acos(-1) / 180 ;
    mat << cos(x),sin(x),-sin(x),cos(x);

    std::cout << "initial" <<std::endl;
    int a , b ;
    std::cin >> a >> b ;
    Eigen::Vector2d ini(a,b) ;
    std::cout << "rotate angle:(shunshizhen)" << std::endl;
    int angle;
    std::cin >> angle;
    Eigen::Matrix2d mat_2 =mat.pow(angle);


    Eigen::Vector2d fin = mat * ini ;
    std::cout << "rotate angle:  " << angle << "° ;\nfrom\n" << ini << "\nto\n" << fin << std::endl;
    return 0;
    





}