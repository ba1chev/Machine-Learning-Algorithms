#include "../utils/pair/pair.hpp"
#include "../utils/matrix/matrix.hpp"
#include "../utils/vector/vector.hpp"

float mse(const Matrix<float>& Y, const Matrix<float>& X,
    const Matrix<float>& W, const float b) {
    Matrix<float> helperVectorColumn(X.getCountOfRows(), 1);

    for (size_t i = 0; i < helperVectorColumn.getCountOfRows(); i++) {
        helperVectorColumn[i][0] = 1;
    }

    Matrix<float> error = Y - ((X * W) + b * helperVectorColumn);
    Matrix<float> squaredSum = error.transpose() * error;

    return squaredSum[0][0] / static_cast<float>(X.getCountOfRows());
}
