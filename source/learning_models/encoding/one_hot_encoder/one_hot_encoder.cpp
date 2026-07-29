#include <map>
#include <vector>
#include <algorithm>

#include "source/learning_models/encoding/one_hot_encoder/one_hot_encoder.h"

Matrix<float> OneHotEncoder::encode(const Matrix<std::string>& data) const {
    size_t samples_count = data.get_rows_count();

    std::vector<std::string> categories;
    std::map<std::string, size_t> category_index;

    for (size_t i = 0; i < samples_count; i++) {
        const std::string& val = data[i][0];
        if (category_index.find(val) == category_index.end()) {
            category_index[val] = 0;
            categories.push_back(val);
        }
    }

    std::sort(categories.begin(), categories.end());
    for (size_t i = 0; i < categories.size(); i++) {
        category_index[categories[i]] = i;
    }

    size_t k = categories.size();
    Matrix<float> result;
    result.resize_dims(samples_count, k);

    for (size_t i = 0; i < samples_count; i++) {
        size_t col = category_index[data[i][0]];
        for (size_t j = 0; j < k; j++) {
            result[i][j] = (j == col) ? 1.f : 0.f;
        }
    }

    return result;
}

std::unique_ptr<EncoderTemplate<Matrix<std::string>, Matrix<float>>> OneHotEncoder::clone() const {
    return std::make_unique<OneHotEncoder>(*this);
}

EncoderType OneHotEncoder::get_type() const {
    return EncoderType::ONE_HOT_ENCODER;
}
