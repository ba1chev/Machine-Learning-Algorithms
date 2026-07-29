#include <fstream>
#include <stdexcept>

#include "source/utils.hpp"
#include "source/build/model_io/text_model_serializer/text_model_serializer.h"

TextModelSerializer::TextModelSerializer(const std::string& filename) : ModelSerializer(filename) {}

void TextModelSerializer::save(LearningModelType type, size_t degree, const Matrix<float>& weights,
    bool has_scaler, ScalerType scaler_type, const Matrix<float>& scaler_parameters) const {
    std::ofstream os(this->filename);
    if (!os) {
        throw std::runtime_error("Could not open model file for writing");
    }

    size_t rows_count = weights.get_rows_count();
    size_t cols_count = weights.get_cols_count();

    os << INPUT::type_to_string(type) << '\n';
    os << degree << '\n';

    if (has_scaler) {
        size_t param_rows = scaler_parameters.get_rows_count();
        size_t param_cols = scaler_parameters.get_cols_count();

        os << INPUT::scaler_type_to_string(scaler_type) << '\n';
        os << param_rows << ' ' << param_cols << '\n';
        for (size_t i = 0; i < param_rows; i++) {
            for (size_t j = 0; j < param_cols; j++) {
                os << scaler_parameters[i][j] << '\n';
            }
        }
    } else {
        os << "none" << '\n';
        os << 0 << ' ' << 0 << '\n';
    }

    os << rows_count << ' ' << cols_count << '\n';

    for (size_t i = 0; i < rows_count; i++) {
        for (size_t j = 0; j < cols_count; j++) {
            os << weights[i][j] << '\n';
        }
    }
}

LoadedModel TextModelSerializer::load() const {
    std::ifstream is(this->filename);
    if (!is) {
        throw std::runtime_error("Could not open model file for reading");
    }

    std::string type_string;
    is >> type_string;

    LoadedModel model;
    model.type = INPUT::parse_type(type_string.c_str());
    is >> model.degree;

    std::string scaler_string;
    is >> scaler_string;

    size_t param_rows = 0;
    size_t param_cols = 0;
    is >> param_rows >> param_cols;

    if (INPUT::is_scaler_none(scaler_string.c_str())) {
        model.has_scaler = false;
    } else {
        model.has_scaler = true;
        model.scaler_type = INPUT::parse_scaler_type(scaler_string.c_str());
        model.scaler_parameters.resize_dims(param_rows, param_cols);
        for (size_t i = 0; i < param_rows; i++) {
            for (size_t j = 0; j < param_cols; j++) {
                if (!(is >> model.scaler_parameters[i][j])) {
                    throw std::runtime_error("Malformed model file: not enough scaler parameters");
                }
            }
        }
    }

    size_t rows_count = 0;
    size_t cols_count = 0;
    is >> rows_count >> cols_count;

    model.weights.resize_dims(rows_count, cols_count);
    for (size_t i = 0; i < rows_count; i++) {
        for (size_t j = 0; j < cols_count; j++) {
            if (!(is >> model.weights[i][j])) {
                throw std::runtime_error("Malformed model file: not enough weight values");
            }
        }
    }

    return model;
}

ModelSerializerType TextModelSerializer::get_type() const {
    return ModelSerializerType::TEXT_MODEL_SERIALIZER;
}
