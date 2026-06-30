#include <cmath>
#include <vector>
#include <stdexcept>

std::vector<std::vector<float>> X = { { 2.5f, 2.4f }, { 0.5f, 0.7f }, { 2.2f, 2.9f }, { 1.9f, 2.2f },
    { 3.1f, 3.0f }, { 2.3f, 2.7f }, { 2.0f, 1.6f }, { 1.0f, 1.1f }, { 1.5f, 1.6f }, { 1.1f, 0.9f } };
size_t pca_count_of_components = 1;
std::vector<float> pca_means;
std::vector<std::vector<float>> pca_components;

std::vector<std::vector<float>> jacobi_eigendecomposition(std::vector<std::vector<float>> matrix,
    std::vector<float>& eigenvalues) {
    size_t size = matrix.size();
    std::vector<std::vector<float>> eigenvectors(size, std::vector<float>(size, 0));

    for (size_t i = 0; i < size; i++) {
        eigenvectors[i][i] = 1;
    }

    const size_t max_iterations = 100;
    const float convergence_threshold = 1e-7f;

    for (size_t iteration = 0; iteration < max_iterations; iteration++) {
        size_t pivot_row = 0;
        size_t pivot_column = 1;
        float pivot_value = std::abs(matrix[0][1]);

        for (size_t i = 0; i < size; i++) {
            for (size_t j = i + 1; j < size; j++) {
                if (std::abs(matrix[i][j]) > pivot_value) {
                    pivot_value = std::abs(matrix[i][j]);
                    pivot_row = i;
                    pivot_column = j;
                }
            }
        }

        if (pivot_value < convergence_threshold) {
            break;
        }

        float diagonal_difference = matrix[pivot_column][pivot_column] - matrix[pivot_row][pivot_row];
        float angle = 0.5f * std::atan2(2 * matrix[pivot_row][pivot_column], diagonal_difference);
        float cosine_value = std::cos(angle);
        float sine_value = std::sin(angle);

        for (size_t i = 0; i < size; i++) {
            float row_value = matrix[i][pivot_row];
            float column_value = matrix[i][pivot_column];
            matrix[i][pivot_row] = cosine_value * row_value - sine_value * column_value;
            matrix[i][pivot_column] = sine_value * row_value + cosine_value * column_value;
        }

        for (size_t j = 0; j < size; j++) {
            float row_value = matrix[pivot_row][j];
            float column_value = matrix[pivot_column][j];
            matrix[pivot_row][j] = cosine_value * row_value - sine_value * column_value;
            matrix[pivot_column][j] = sine_value * row_value + cosine_value * column_value;
        }

        for (size_t i = 0; i < size; i++) {
            float row_value = eigenvectors[i][pivot_row];
            float column_value = eigenvectors[i][pivot_column];
            eigenvectors[i][pivot_row] = cosine_value * row_value - sine_value * column_value;
            eigenvectors[i][pivot_column] = sine_value * row_value + cosine_value * column_value;
        }
    }

    eigenvalues.assign(size, 0);

    for (size_t i = 0; i < size; i++) {
        eigenvalues[i] = matrix[i][i];
    }

    return eigenvectors;
}

void pca_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_samples = input.size();
    size_t count_of_features = input[0].size();

    if (pca_count_of_components > count_of_features) {
        throw std::runtime_error("Invalid input data");
    }

    pca_means.assign(count_of_features, 0);

    for (size_t i = 0; i < count_of_samples; i++) {
        if (input[i].size() != count_of_features) {
            throw std::runtime_error("Invalid input data");
        }

        for (size_t j = 0; j < count_of_features; j++) {
            pca_means[j] += input[i][j];
        }
    }

    for (size_t j = 0; j < count_of_features; j++) {
        pca_means[j] /= count_of_samples;
    }

    std::vector<std::vector<float>> covariance(count_of_features, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < count_of_samples; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            for (size_t k = 0; k < count_of_features; k++) {
                covariance[j][k] += (input[i][j] - pca_means[j]) * (input[i][k] - pca_means[k]);
            }
        }
    }

    for (size_t j = 0; j < count_of_features; j++) {
        for (size_t k = 0; k < count_of_features; k++) {
            covariance[j][k] /= count_of_samples;
        }
    }

    std::vector<float> eigenvalues;
    std::vector<std::vector<float>> eigenvectors = jacobi_eigendecomposition(covariance, eigenvalues);

    std::vector<size_t> sorted_indices(count_of_features);

    for (size_t i = 0; i < count_of_features; i++) {
        sorted_indices[i] = i;
    }

    for (size_t i = 0; i < count_of_features; i++) {
        for (size_t j = i + 1; j < count_of_features; j++) {
            if (eigenvalues[sorted_indices[j]] > eigenvalues[sorted_indices[i]]) {
                size_t temporary_index = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temporary_index;
            }
        }
    }

    pca_components.assign(pca_count_of_components, std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < pca_count_of_components; i++) {
        size_t column = sorted_indices[i];

        for (size_t j = 0; j < count_of_features; j++) {
            pca_components[i][j] = eigenvectors[j][column];
        }
    }
}

std::vector<float> pca(const std::vector<float>& input) {
    if (input.size() != pca_means.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(pca_count_of_components, 0);

    for (size_t i = 0; i < pca_count_of_components; i++) {
        for (size_t j = 0; j < input.size(); j++) {
            result[i] += (input[j] - pca_means[j]) * pca_components[i][j];
        }
    }

    return result;
}

int main() {
    pca_fit(X);
    std::vector<float> projection = pca({ 2.0f, 2.1f });

    return 0;
}
