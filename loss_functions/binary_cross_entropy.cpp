#include <cmath>
#include "../utils/matrix/matrix.hpp"

float sigmoid(const float number) {
    return 1 / (1 + std::exp(-number));
}

float binary_cross_entropy(const Matrix<float>& Y, const Matrix<float>& X,
    const Matrix<float>& W, const float b) {
    Matrix<float> helperVectorColumn(X.getCountOfRows(), 1);

    for (size_t i = 0; i < helperVectorColumn.getCountOfRows(); i++) {
        helperVectorColumn[i][0] = 1;
    }

    Matrix<float> Z = (X * W) + b * helperVectorColumn;
    const float epsilon = 0.000000001;
    float lossSum = 0;

    for (size_t i = 0; i < Z.getCountOfRows(); i++) {
        float yHatPrediction = sigmoid(Z[i][0]);

        if (yHatPrediction < epsilon) {
            yHatPrediction = epsilon;
        } else if (yHatPrediction > 1 - epsilon) {
            yHatPrediction = 1 - epsilon;
        }

        const float y = Y[i][0];
        lossSum += y * std::log(yHatPrediction) + (1 - y) * std::log(1 - yHatPrediction);
    }

    return -lossSum / static_cast<float>(X.getCountOfRows());
}
