#pragma once

#include "source/learning_models/scaling/scaler.hpp"
#include "source/data/data_structures/matrix/matrix.hpp"
#include "source/data/input_output/csv/csv_table/csv_table.h"

namespace BUILD_DETAIL {
    Matrix<float> read_matrix(const char* path);
    Matrix<float> extract_features(const Matrix<float>& raw_matrix);

    void assemble(const Matrix<float>& features, const Matrix<float>& raw_matrix, bool add_bias,
        Matrix<float>& X_out, Matrix<float>& y_out);
}

namespace BUILD {
    CSVTable matrix_to_table(const Matrix<float>& matrix);

    void split_features_target(const char* path, bool add_bias, Matrix<float>& X_out,
        Matrix<float>& y_out, Scaler<float>* scaler = nullptr);

    void split_features_target_scaled(const char* path, bool add_bias, Matrix<float>& X_out,
        Matrix<float>& y_out, const Scaler<float>* fitted_scaler);

    void read_features(const char* path, Matrix<float>& X_out, Scaler<float>* scaler = nullptr);

    void read_features_scaled(const char* path, Matrix<float>& X_out, const Scaler<float>* fitted_scaler);
}
