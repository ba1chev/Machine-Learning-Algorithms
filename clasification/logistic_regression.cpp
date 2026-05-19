#include <cmath>
#include "../utils/pair/pair.hpp"
#include "../utils/matrix/matrix.hpp"
#include "../utils/vector/vector.hpp"

float sigmoid(const float number) {
    return 1 / (1 + std::exp(-number));
}

Matrix<float> calculate_predictions(const Matrix<float>& X, const Matrix<float>& W, const float b) {
    Matrix<float> helperVectorColumn(X.getCountOfRows(), 1);

    for (size_t i = 0; i < helperVectorColumn.getCountOfRows(); i++) {
        helperVectorColumn[i][0] = 1;
    }

    Matrix<float> Z = (X * W) + b * helperVectorColumn;
    Matrix<float> Y_hat(Z.getCountOfRows(), 1);

    for (size_t i = 0; i < Z.getCountOfRows(); i++) {
        Y_hat[i][0] = sigmoid(Z[i][0]);
    }

    return Y_hat;
}

Matrix<float> calculate_error_vector(const Matrix<float>& Y, const Matrix<float>& X,
    const Matrix<float>& W, const float b) {
    Matrix<float> Y_hat = calculate_predictions(X, W, b);
    return Y_hat - Y;
}

Pair<Matrix<float>, float> gradient_descent(const Matrix<float>& Y, const Matrix<float>& X,
    const Matrix<float>& W, const float b, const float learningRate = 0.00001) {
    Matrix<float> errorMatrix = calculate_error_vector(Y, X, W, b);
    const float regressionScalling = 1 / static_cast<float>(X.getCountOfRows());
    Matrix<float> gradientW = regressionScalling * (X.transpose() * errorMatrix);
    float errorSum = 0;

    for (size_t i = 0; i < errorMatrix.getCountOfRows(); i++) {
        errorSum += errorMatrix[i][0];
    }

    float gradientB = regressionScalling * errorSum;

    Matrix<float> updatedW = W - learningRate * gradientW;
    float updatedB = b - learningRate * gradientB;
    return Pair<Matrix<float>, float>(updatedW, updatedB);
}

void logistic_regression_simulation(size_t numberOfIterations, const Matrix<float>& Y, const Matrix<float>& X,
    Matrix<float>& W, float& b, const float learningRate = 0.00001) {
    for (size_t i = 0; i < numberOfIterations; i++) {
        Pair<Matrix<float>, float> currentInterationResult = gradient_descent(Y, X, W, b, learningRate);
        W = currentInterationResult.getFirst();
        b = currentInterationResult.getSecond();
    }
}
