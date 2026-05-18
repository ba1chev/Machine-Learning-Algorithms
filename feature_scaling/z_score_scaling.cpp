#include <cmath>
#include "../utils/pair/pair.hpp"
#include "../utils/matrix/matrix.hpp"
#include "../utils/vector/vector.hpp"

float power_2(float number) {
    return number * number;
}

float get_mean_value_by_i_feature(const Matrix<float>& matrix, size_t index) {
    if (matrix.getCountOfRows() == 0 || index >= matrix.getCountOfCols()) {
        throw std::runtime_error("The matrix is empty or index is out of range");
    }

    float result = 0;
    for (size_t i = 0; i < matrix.getCountOfRows(); i++) {
        result += matrix[i][index];
    }
    
    return result / matrix.getCountOfRows();
}

float get_variation_by_i_feature(const Matrix<float>& matrix, size_t index) {
    if (matrix.getCountOfRows() == 0 || index >= matrix.getCountOfCols()) {
        throw std::runtime_error("The matrix is empty or index is out of range");
    }

    Matrix<float> transformedMatrix = matrix;
    float meanValue = get_mean_value_by_i_feature(matrix, index);
    
    for (size_t i = 0; i < transformedMatrix.getCountOfRows(); i++) { 
        transformedMatrix[i][index] = power_2(transformedMatrix[i][index] - meanValue);
    }
    
    return get_mean_value_by_i_feature(transformedMatrix, index);
}

void z_score_scaling(Matrix<float>& matrix) {
    for (size_t i = 0; i < matrix.getCountOfCols(); i++) {
        float currentFeatureMeanValue = get_mean_value_by_i_feature(matrix, i);
        float currentFeatureVarianceValue = get_variation_by_i_feature(matrix, i);

        for (size_t j = 0; j < matrix.getCountOfRows(); j++) {
            if (currentFeatureVarianceValue == 0) {
                matrix[j][i] = 0;
                continue;
            }
            matrix[j][i] = (matrix[j][i] - currentFeatureMeanValue) / std::sqrt(currentFeatureVarianceValue);
        }
    }
}