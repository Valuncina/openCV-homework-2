#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <ceres/ceres.h>

struct CostFunctor{
    template <typename A>
    bool operator()(const A* const x, A* residual) const {
        residual[0] = A(10.0) - x[0];
        return true;
    }

};
//3.5

int main(){
    const double pi = std::acos(-1);
    Eigen::AngleAxisd rotation(pi / 2.0, Eigen::Vector3d::UnitZ());
    Eigen::Matrix3d R = rotation.toRotationMatrix();
    Eigen::Vector3d t(1, 2, 0), p(1, 0, 0);
    Eigen::Vector3d p1 = R * p + t;
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();//rigid body
    T.linear() = R;
    T.translation()= t;
    //2.10
    cv::Mat cvR;
    cv::eigen2cv(R, cvR);
    //2.11

    ceres::Problem problem;
    //3.3
    double x = 0.0;
    auto* cost = new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
    problem.AddResidualBlock(cost, nullptr, &x);
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";
    std::cout<< "Final x = " << x << "\n";
    return 0;


}
