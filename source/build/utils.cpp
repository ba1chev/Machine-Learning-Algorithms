#include <err.h>
#include <memory>

#include "source/build/utils.h"
#include "source/data/preprocessing/data_preprocessor.h"
#include "source/data/input_output/readers/reader_factory/reader_factory.h"
#include "source/data/input_output/csv/csv_field/csv_field_num/csv_field_num.h"
#include "source/data/input_output/csv/csv_field/csv_field_factory/csv_field_factory.h"

Matrix<float> BUILD_DETAIL::read_matrix(const char* path) {
    std::unique_ptr<Reader> reader = ReaderFactory().create(ReaderType::CSV_READER, path);
    reader->read();

    DataPreprocessor preprocessor(reader.get());
    return preprocessor.preprocess();
}

void BUILD_DETAIL::assemble(const Matrix<float>& features, const Matrix<float>& raw_matrix, bool add_bias, Matrix<float>& X_out, Matrix<float>& y_out) {
    size_t rows_count = features.get_rows_count();
    size_t feature_count = features.get_cols_count();
    size_t cols_count = raw_matrix.get_cols_count();

    X_out.resize_dims(rows_count, add_bias ? feature_count + 1 : feature_count);
    y_out.resize_dims(rows_count, 1);

    for (size_t i = 0; i < rows_count; i++) {
        for (size_t j = 0; j < feature_count; j++) {
            X_out[i][j] = features[i][j];
        }
        if (add_bias) {
            X_out[i][feature_count] = 1;
        }
        y_out[i][0] = raw_matrix[i][cols_count - 1];
    }
}

Matrix<float> BUILD_DETAIL::extract_features(const Matrix<float>& raw_matrix) {
    size_t rows_count = raw_matrix.get_rows_count();
    size_t cols_count = raw_matrix.get_cols_count();
    size_t feature_count = cols_count - 1;

    Matrix<float> features;
    features.resize_dims(rows_count, feature_count);
    for (size_t i = 0; i < rows_count; i++) {
        for (size_t j = 0; j < feature_count; j++) {
            features[i][j] = raw_matrix[i][j];
        }
    }

    return features;
}

void BUILD::split_features_target(const char* path, bool add_bias, Matrix<float>& X_out, Matrix<float>& y_out, Scaler<float>* scaler) {
    Matrix<float> parsed_matrix = BUILD_DETAIL::read_matrix(path);
    if (parsed_matrix.get_cols_count() < 2) {
        errx(1, "CSV must have at least one feature column and a target column");
    }

    Matrix<float> features = BUILD_DETAIL::extract_features(parsed_matrix);
    if (scaler) {
        scaler->fit(features);
        features = scaler->transform(features);
    }

    BUILD_DETAIL::assemble(features, parsed_matrix, add_bias, X_out, y_out);
}

void BUILD::split_features_target_scaled(const char* path, bool add_bias, Matrix<float>& X_out, Matrix<float>& y_out, const Scaler<float>* fitted_scaler) {
    Matrix<float> parsed_matrix = BUILD_DETAIL::read_matrix(path);
    if (parsed_matrix.get_cols_count() < 2) {
        errx(1, "CSV must have at least one feature column and a target column");
    }

    Matrix<float> features = BUILD_DETAIL::extract_features(parsed_matrix);
    if (fitted_scaler) {
        features = fitted_scaler->transform(features);
    }

    BUILD_DETAIL::assemble(features, parsed_matrix, add_bias, X_out, y_out);
}

void BUILD::read_features(const char* path, Matrix<float>& X_out, Scaler<float>* scaler) {
    Matrix<float> parsed_matrix = BUILD_DETAIL::read_matrix(path);
    if (parsed_matrix.get_cols_count() < 1) {
        errx(1, "CSV must have at least one feature column");
    }

    if (scaler) {
        scaler->fit(parsed_matrix);
        parsed_matrix = scaler->transform(parsed_matrix);
    }

    X_out = parsed_matrix;
}

void BUILD::read_features_scaled(const char* path, Matrix<float>& X_out, const Scaler<float>* fitted_scaler) {
    Matrix<float> parsed_matrix = BUILD_DETAIL::read_matrix(path);
    if (parsed_matrix.get_cols_count() < 1) {
        errx(1, "CSV must have at least one feature column");
    }

    if (fitted_scaler) {
        parsed_matrix = fitted_scaler->transform(parsed_matrix);
    }

    X_out = parsed_matrix;
}

CSVTable BUILD::matrix_to_table(const Matrix<float>& matrix) {
    CSVTable table;
    size_t rows_count = matrix.get_rows_count();
    size_t cols_count = matrix.get_cols_count();

    for (size_t i = 0; i < rows_count; i++) {
        CSVRow row;

        for (size_t j = 0; j < cols_count; j++) {
            std::unique_ptr<CSVField> field = CSVFieldFactory().create(CSVFieldType::CSV_FIELD_NUM);
            static_cast<CSVFieldNUM*>(field.get())->field_data = matrix[i][j];
            row.add_field(field.get());
        }
        table.add_row(row);
    }

    return table;
}
