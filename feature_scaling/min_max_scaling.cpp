#include "../utils/pair/pair.hpp"
#include "../utils/matrix/matrix.hpp"
#include "../utils/vector/vector.hpp"

float get_max_value_by_i_feature(const Matrix<float>& matrix, size_t index) {
    if (matrix.getCountOfRows() == 0 || index >= matrix.getCountOfCols()) {
        throw std::runtime_error("The matrix is empty or index is out of range");
    }

    float result = matrix[0][index];
    
    for (size_t i = 0; i < matrix.getCountOfRows(); i++) {
        if (result <= matrix[i][index]) {
            result = matrix[i][index];
        }    
    }
    
    return result;
}

float get_min_value_by_i_feature(const Matrix<float>& matrix, size_t index) {
    if (matrix.getCountOfRows() == 0 || index >= matrix.getCountOfCols()) {
        throw std::runtime_error("The matrix is empty or index is out of range");
    }

    float result = matrix[0][index];
    
    for (size_t i = 0; i < matrix.getCountOfRows(); i++) {
        if (result >= matrix[i][index]) {
            result = matrix[i][index];
        }    
    }
    
    return result;
}

void min_max_scaling(Matrix<float>& matrix) {
    for (size_t i = 0; i < matrix.getCountOfCols(); i++) {
        float currentFeatureMinValue = get_min_value_by_i_feature(matrix, i);
        float currentFeatureMaxValue = get_max_value_by_i_feature(matrix, i);

        for (size_t j = 0; j < matrix.getCountOfRows(); j++) {
            if (currentFeatureMinValue == currentFeatureMaxValue) {
                matrix[j][i] = 0;
                continue;
            }
            matrix[j][i] = (matrix[j][i] - currentFeatureMinValue) / (currentFeatureMaxValue - currentFeatureMinValue);
        }
    }
}