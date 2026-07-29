#include <map>
#include <vector>
#include <algorithm>

#include "source/learning_models/encoding/ordinal_encoder/ordinal_encoder.h"

Matrix<float> OrdinalEncoder::encode(const Matrix<std::string>& data) const {
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

    Matrix<float> result;
    result.resize_dims(samples_count, 1);

    for (size_t i = 0; i < samples_count; i++) {
        result[i][0] = (float)category_index[data[i][0]];
    }

    return result;
}

std::unique_ptr<EncoderTemplate<Matrix<std::string>, Matrix<float>>> OrdinalEncoder::clone() const {
    return std::make_unique<OrdinalEncoder>(*this);
}

EncoderType OrdinalEncoder::get_type() const {
    return EncoderType::ORDINAL_ENCODER;
}
