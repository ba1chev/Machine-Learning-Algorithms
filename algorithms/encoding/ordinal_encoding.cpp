#include <vector>
#include <string>
#include <stdexcept>

std::vector<std::string> X = { "low", "high", "medium", "low", "high", "medium", "low" };
std::vector<std::string> ordinal_encoding_categories = { "low", "medium", "high" };

size_t find_category_index(const std::string& category) {
    for (size_t i = 0; i < ordinal_encoding_categories.size(); i++) {
        if (ordinal_encoding_categories[i] == category) {
            return i;
        }
    }

    return ordinal_encoding_categories.size();
}

std::vector<size_t> ordinal_encoding_fit(const std::vector<std::string>& input) {
    std::vector<size_t> result(input.size(), 0);

    for (size_t i = 0; i < input.size(); i++) {
        size_t index = find_category_index(input[i]);

        if (index == ordinal_encoding_categories.size()) {
            throw std::runtime_error("Invalid input data");
        }

        result[i] = index;
    }

    return result;
}

size_t ordinal_encoding(const std::string& input) {
    size_t index = find_category_index(input);

    if (index == ordinal_encoding_categories.size()) {
        throw std::runtime_error("Invalid input data");
    }

    return index;
}

int main() {
    std::vector<size_t> encoded = ordinal_encoding_fit(X);
    return static_cast<int>(ordinal_encoding("medium"));
}
