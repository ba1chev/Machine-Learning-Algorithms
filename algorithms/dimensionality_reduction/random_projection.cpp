#include <cmath>
#include <vector>
#include <stdexcept>
#include <cstdlib>

std::vector<std::vector<float>> X = { { 2.5f, 2.4f, 1.2f, 0.8f }, { 0.5f, 0.7f, 0.9f, 1.1f },
    { 2.2f, 2.9f, 1.5f, 0.6f }, { 1.9f, 2.2f, 1.0f, 0.9f }, { 3.1f, 3.0f, 1.8f, 0.4f } };
size_t random_projection_count_of_components = 2;
std::vector<std::vector<float>> random_projection_matrix;

float sample_gaussian() {
    float uniform_left = (std::rand() + 1.0f) / ((float)RAND_MAX + 2);
    float uniform_right = (std::rand() + 1.0f) / ((float)RAND_MAX + 2);
    return std::sqrt(-2.0f * std::log(uniform_left)) * std::cos(2.0f * M_PI * uniform_right);
}

void random_projection_fit(const std::vector<std::vector<float>>& input) {
    if (input.empty()) {
        throw std::runtime_error("Invalid input data");
    }

    size_t count_of_features = input[0].size();

    for (size_t i = 1; i < input.size(); i++) {
        if (input[i].size() != count_of_features) {
            throw std::runtime_error("Invalid input data");
        }
    }

    float scaling_factor = 1.0f / std::sqrt((float)random_projection_count_of_components);
    random_projection_matrix.assign(random_projection_count_of_components,
        std::vector<float>(count_of_features, 0));

    for (size_t i = 0; i < random_projection_count_of_components; i++) {
        for (size_t j = 0; j < count_of_features; j++) {
            random_projection_matrix[i][j] = scaling_factor * sample_gaussian();
        }
    }
}

std::vector<float> random_projection(const std::vector<float>& input) {
    if (random_projection_matrix.empty() || input.size() != random_projection_matrix[0].size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(random_projection_count_of_components, 0);

    for (size_t i = 0; i < random_projection_count_of_components; i++) {
        for (size_t j = 0; j < input.size(); j++) {
            result[i] += input[j] * random_projection_matrix[i][j];
        }
    }

    return result;
}

int main() {
    random_projection_fit(X);
    std::vector<float> projection = random_projection({ 2.0f, 2.1f, 1.3f, 0.7f });

    return 0;
}
