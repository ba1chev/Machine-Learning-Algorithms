#include <vector>
#include <string>
#include <stdexcept>

std::vector<std::string> X = { "red", "blue", "green", "blue", "red", "green", "yellow" };
std::vector<std::string> label_encoding_categories;

size_t find_category_index(const std::string& category) {
    for (size_t i = 0; i < label_encoding_categories.size(); i++) {
        if (label_encoding_categories[i] == category) {
            return i;
        }
    }

    return label_encoding_categories.size();
}

std::vector<size_t> label_encoding_fit(const std::vector<std::string>& input) {
    label_encoding_categories.clear();
    std::vector<size_t> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);

        if (index == label_encoding_categories.size()) {
            label_encoding_categories.push_back(input[i]);
        }

        result[i] = index;
    }

    return result;
}

size_t label_encoding(const std::string& input) {
    size_t index = find_category_index(input);

    if (index == label_encoding_categories.size()) {
        throw std::runtime_error("Invalid input data");
    }

    return index;
}

int main() {
    std::vector<size_t> encoded = label_encoding_fit(X);
    return static_cast<int>(label_encoding("green"));
}
