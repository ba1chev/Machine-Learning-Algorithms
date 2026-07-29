#include <cmath>
#include <cassert>
#include <iostream>

#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/learning_models/metrics/classification/recall/recall.hpp"
#include "source/learning_models/metrics/regression/r_squared/r_squared.hpp"
#include "source/learning_models/metrics/classification/f1_score/f1_score.hpp"
#include "source/learning_models/metrics/classification/accuracy/accuracy.hpp"
#include "source/learning_models/metrics/classification/precision/precision.hpp"
#include "source/learning_models/metrics/regression/mean_squared_error/mean_squared_error.hpp"
#include "source/learning_models/metrics/regression/mean_absolute_error/mean_absolute_error.hpp"
#include "source/learning_models/metrics/regression/root_mean_squared_error/root_mean_squared_error.hpp"

static bool close(double a, double b) {
    return std::fabs(a - b) < 1e-5;
}

static Matrix<double> column(const std::vector<double>& values) {
    Matrix<double> m;
    m.resize_dims(values.size(), 1);
    for (size_t i = 0; i < values.size(); i++) {
        m[i][0] = values[i];
    }
    return m;
}

int metrics_test() {
    // Regression: y_true = [3, -0.5, 2, 7], y_pred = [2.5, 0.0, 2, 8]
    // errors = [0.5, -0.5, 0, -1] -> squared = [0.25, 0.25, 0, 1] -> MSE = 1.5/4 = 0.375
    Matrix<double> y_true = column({3, -0.5, 2, 7});
    Matrix<double> y_pred = column({2.5, 0.0, 2, 8});

    assert(close(MeanSquaredError<double>().compute(y_true, y_pred), 0.375));
    assert(close(RootMeanSquaredError<double>().compute(y_true, y_pred), std::sqrt(0.375)));
    // MAE = (0.5 + 0.5 + 0 + 1) / 4 = 0.5
    assert(close(MeanAbsoluteError<double>().compute(y_true, y_pred), 0.5));
    // R2: mean(y_true) = 2.875; SS_tot = 0.015625+11.390625+0.765625+17.015625 = 29.1875
    //     SS_res = 0.25+0.25+0+1 = 1.5 -> R2 = 1 - 1.5/29.1875
    assert(close(RSquared<double>().compute(y_true, y_pred), 1.0 - 1.5 / 29.1875));

    // Classification: actual = [1,1,0,0,1,0], pred = [1,0,0,1,1,0]
    // TP=2 (idx0,4), FP=1 (idx3), FN=1 (idx1), TN=2 (idx2,5)
    Matrix<double> c_true = column({1, 1, 0, 0, 1, 0});
    Matrix<double> c_pred = column({1, 0, 0, 1, 1, 0});

    // Accuracy = correct(4) / 6
    assert(close(Accuracy<double>().compute(c_true, c_pred), 4.0 / 6.0));
    // Precision = TP / (TP+FP) = 2/3
    assert(close(Precision<double>().compute(c_true, c_pred), 2.0 / 3.0));
    // Recall = TP / (TP+FN) = 2/3
    assert(close(Recall<double>().compute(c_true, c_pred), 2.0 / 3.0));
    // F1 = 2*P*R/(P+R) = 2/3
    assert(close(F1Score<double>().compute(c_true, c_pred), 2.0 / 3.0));

    std::cout << "metrics_test: all assertions passed" << std::endl;
    return 0;
}

int main() {
    return metrics_test();
}
