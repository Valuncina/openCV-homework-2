#include <iostream>
#include <cmath>
#include <ceres/ceres.h>
#include <random>
#include <vector>
//sigmoid function
struct ExponentialResidual {
    ExponentialResidual(double x, double y)
        : x_(x), y_(y){}
    template <typename T>
    bool operator()(const T* const ab, T* residual) const {
        using std::exp;
        residual[0] = T(y_) - T(1.0)/(T(1.0)+exp(ab[0] * T(x_) + ab[1])); 
        return true;

    }
    double x_, y_;
};

int main(){
    std::vector<double> x_data, y_data;
    const double a_true = 0.3, b_true = 0.1;
    std::mt19937 gen(67);//676767
    std::normal_distribution<double> noise(0.0, 0.02);
    for(int i = 0; i < 100; ++i){
        double x = i / 10.0;
        x_data.push_back(x);
        y_data.push_back(1.0 / (1.0 + std::exp(a_true * x + b_true)) + noise(gen));
    }

    double ab[2] = {0.0, 0.0};
    ceres::Problem problem;
    for (size_t i = 0; i < x_data.size(); ++i) {
        auto* cost = new ceres::AutoDiffCostFunction<
            ExponentialResidual, 1, 2>(new ExponentialResidual(
                x_data[i], y_data[i]));
        problem.AddResidualBlock(cost, new ceres::HuberLoss(1.0), ab);//using huberloss

    }
    
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";
    std::cout<< "Final a,b = " << ab[0] << "," 
        << ab[1] << "\n";

    return 0;
}