#include <vector>
#include <string>
#include <stdexcept>

std::vector<std::string> X = { "red", "blue", "green", "blue", "red", "green", "yellow" };
std::vector<std::string> one_hot_encoding_categories;

size_t find_category_index(const std::string& category) {
    for (size_t i = 0; i < one_hot_encoding_categories.size(); i++) {
        if (one_hot_encoding_categories[i] == category) {
            return i;
        }
    }

    return one_hot_encoding_categories.size();
}

std::vector<std::vector<float>> one_hot_encoding_fit(const std::vector<std::string>& input) {
    one_hot_encoding_categories.clear();

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);

        if (index == one_hot_encoding_categories.size()) {
            one_hot_encoding_categories.push_back(input[i]);
        }
    }

    size_t count_of_categories = one_hot_encoding_categories.size();
    std::vector<std::vector<float>> result(input.size(), std::vector<float>(count_of_categories, 0));

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);
        result[i][index] = 1;
    }

    return result;
}

std::vector<float> one_hot_encoding(const std::string& input) {
    size_t index = find_category_index(input);

    if (index == one_hot_encoding_categories.size()) {
        throw std::runtime_error("Invalid input data");
    }

    std::vector<float> result(one_hot_encoding_categories.size(), 0);
    result[index] = 1;
    return result;
}

int main() {
    std::vector<std::vector<float>> encoded = one_hot_encoding_fit(X);
    std::vector<float> prediction = one_hot_encoding("green");

    return 0;
}
