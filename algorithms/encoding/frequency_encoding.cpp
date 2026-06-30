#include <vector>
#include <string>
#include <stdexcept>

std::vector<std::string> X = { "red", "blue", "green", "blue", "red", "green", "yellow", "blue" };
std::vector<std::string> frequency_encoding_categories;
std::vector<size_t> frequency_encoding_counts;

size_t find_category_index(const std::string& category) {
    for (size_t i = 0; i < frequency_encoding_categories.size(); i++) {
        if (frequency_encoding_categories[i] == category) {
            return i;
        }
    }

    return frequency_encoding_categories.size();
}

std::vector<size_t> frequency_encoding_fit(const std::vector<std::string>& input) {
    frequency_encoding_categories.clear();
    frequency_encoding_counts.clear();

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);

        if (index == frequency_encoding_categories.size()) {
            frequency_encoding_categories.push_back(input[i]);
            frequency_encoding_counts.push_back(1);
        }
        else {
            frequency_encoding_counts[index]++;
        }
    }

    std::vector<size_t> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);
        result[i] = frequency_encoding_counts[index];
    }

    return result;
}

size_t frequency_encoding(const std::string& input) {
    size_t index = find_category_index(input);

    if (index == frequency_encoding_categories.size()) {
        throw std::runtime_error("Invalid input data");
    }

    return frequency_encoding_counts[index];
}

int main() {
    std::vector<size_t> encoded = frequency_encoding_fit(X);
    return static_cast<int>(frequency_encoding("blue"));
}
