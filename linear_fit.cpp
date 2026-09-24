#include <iostream>
#include <Eigen/Dense>

int main(){
    Eigen::Matrix<double, 6, 2> A;
    A << 1, 1,
         2, 1,
         3,	1,
         4, 1,
         5,	1,
         6,	1;
    
    Eigen::VectorXd y(6);
    y << 3.2, 4.8, 7.1, 8.9, 11.2, 12.8;
    //expect y=2x+1
    //y=wx+b => y=Aθ, find θ[a,b]
    Eigen::Vector2d theta = A.colPivHouseholderQr().solve(y);
    Eigen::VectorXd residual(6);
    residual = A * theta - y;
    double value = residual.squaredNorm();
    std::cout << "w = " << theta[0] << "\n";
    std::cout << "b = " << theta[1] << "\n";
    std::cout << "SSE = " << value << "\n";
    return 0;


}
//拟合出w=1.97
    //b=1.1
    //SSE=0.166