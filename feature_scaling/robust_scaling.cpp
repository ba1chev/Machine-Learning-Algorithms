#include <cmath>
#include "../utils/pair/pair.hpp"
#include "../utils/matrix/matrix.hpp"
#include "../utils/vector/vector.hpp"

void bubbleSort(float* data, size_t size) {
    if (data == nullptr) {
        throw std::runtime_error("Nullptr detected");
    }

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
}

float get_median_value(const Matrix<float>& matrix, size_t index) {
    Vector<float> currentFeature = matrix.transpose()[index];
    bubbleSort(currentFeature.getDataByRef(), currentFeature.getSize());

    if (currentFeature.getSize() & (1 << 0)) {
        return currentFeature[currentFeature.getSize() / 2];
    } else {
        return (currentFeature[currentFeature.getSize() / 2 - 1] +
            currentFeature[currentFeature.getSize() / 2]) / 2;
    }
}

float get_p_quantile(const Matrix<float>& matrix, size_t index, float percent) {
    Vector<float> currentFeature = matrix.transpose()[index];
    bubbleSort(currentFeature.getDataByRef(), currentFeature.getSize());

    float h = percent * (currentFeature.getSize() - 1);
    uint32_t integerPart = std::floor(h);
    float floatPart = h - integerPart;

    return currentFeature[integerPart] + floatPart *
        (currentFeature[integerPart + 1] - currentFeature[integerPart]);
}

float get_iqr(const Matrix<float>& matrix, size_t index) {
    return get_p_quantile(matrix, index, 0.75) - get_p_quantile(matrix, index, 0.25);
}

void robust_scaling(Matrix<float>& matrix) {
    for (size_t i = 0; i < matrix.getCountOfCols(); i++) {
        float currentFeatureMedianValue = get_median_value(matrix, i);
        float currentFeatureIQRValue = get_iqr(matrix, i);

        for (size_t j = 0; j < matrix.getCountOfRows(); j++) {
            if (currentFeatureIQRValue == 0) {
                matrix[j][i] = 0;
                continue;
            }
            matrix[j][i] = (matrix[j][i] - currentFeatureMedianValue) / currentFeatureIQRValue;
        }
    }
}