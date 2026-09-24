#include <iostream>
#include <Eigen/Geometry>

#include <Eigen/Dense>

using Eigen::Matrix2d;
using Eigen::MatrixXd;
using Eigen::Vector3d;
using Eigen::Matrix3d;

int main(){

    Matrix3d A, B, M;
    A << 1, 2, 23, 4, 33, 21, -20, 12, 22;
    B << 19, 20, 30, 20, 0, 15, 30, 15,-23;
    MatrixXd sum = A + B;
    MatrixXd product = A * B;
    MatrixXd scaled = 3 * A;
    MatrixXd transposed = A.transpose();
    Matrix2d identity = Matrix2d::Identity();
    MatrixXd zeros = MatrixXd::Zero(3, 4);
    double first = A(0, 0);
    //2.4
    Vector3d u(1, 2, 3);
    double dot = u.dot(u);
    Vector3d cross = u.cross(u);
    double length = u.norm();
    Vector3d unit = u.normalized();
    //2.5
    Matrix3d elementwise = A.cwiseProduct(B);
    Matrix3d squared = A.array().square().matrix();
    Eigen::Array3d values(1.0,2.0,3.0);
    Eigen::Array3d exponentials = values.exp();
    //2.6  
    M << 1, 2, 3,
         2, 4, 6,
         3, 6, 9;
    Vector3d column = M.col(0);
    Eigen::RowVector3d row = M.row(1); 
    Matrix2d block = M.block<2,2>(0,0);
    M.block<2,2>(1,1).setIdentity();
    M.transposeInPlace();
    //2.7
    //question:Ax=u
    Vector3d x = A.colPivHouseholderQr().solve(u);
    double residual = (A * x - u).norm();
    //2.8
    
    
    std::cout << A.rows() << " x " << A.cols() << "\n";
    std::cout << exponentials << std::endl;
    return 0;
}