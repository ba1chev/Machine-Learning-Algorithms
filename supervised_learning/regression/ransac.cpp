#include <cmath>
#include <vector>
#include <stdexcept>
#include <cstdlib>

typedef float(*Function)(const std::vector<float>&);

std::vector<std::vector<float>> X = { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 }, { 10, 10, 10 } };
std::vector<float> Y = { 14, 20, 26, 100 };

float predict(const std::vector<float>& x, const std::vector<float>& params) {
    float result = 0;

    for (size_t i = 0; i < x.size(); i++) {
        result += x[i] * params[i];
    }

    result += params[x.size()];
    return result;
}

size_t ransac_score(const std::vector<std::vector<float>>& X, const std::vector<float>& Y,
    const std::vector<float>& params, float threshold) {
    size_t inliers = 0;

    for (size_t i = 0; i < X.size(); i++) {
        float diff = predict(X[i], params) - Y[i];

        if (diff * diff < threshold) {
            inliers++;
        }
    }

    return inliers;
}

std::vector<float> fit_from_sample(const std::vector<std::vector<float>>& X, const std::vector<float>& Y, int index) {
    size_t dim = X[0].size();
    std::vector<float> params(dim + 1, 0);

    for (size_t j = 0; j < dim; j++) {
        params[j] = X[index][j] * Y[index];
    }

    params[dim] = Y[index];
    return params;
}

std::vector<float> ransac(const std::vector<std::vector<float>>& X, const std::vector<float>& Y,
    int iterations, float threshold) {
    size_t best_score = 0;
    std::vector<float> best_params;

    for (int it = 0; it < iterations; it++) {
        int index = rand() % X.size();
        std::vector<float> params = fit_from_sample(X, Y, index);
        size_t score = ransac_score(X, Y, params, threshold);

        if (score > best_score) {
            best_score = score;
            best_params = params;
        }
    }

    return best_params;
}

int main() {
    std::vector<float> ransac_result = ransac(X, Y, 1000, 10.0f);
    return 0;
}