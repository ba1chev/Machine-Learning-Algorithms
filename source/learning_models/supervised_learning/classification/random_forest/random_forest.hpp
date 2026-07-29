#pragma once

#include <map>
#include <random>
#include <memory>
#include <vector>
#include <stdexcept>

#include "source/learning_models/learning_model_type.h"
#include "source/learning_models/encoding/tree_encoder/tree_encoder.h"
#include "source/learning_models/decoding/tree_decoder/tree_decoder.h"
#include "source/learning_models/supervised_learning/supervised_learning_model.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_tree.hpp"

template <class T>
class RandomForest: public SupervisedLearningModel<T> {
private:
    size_t n_trees;
    size_t max_depth;

    std::vector<Matrix<T>> boot_X;
    std::vector<Matrix<T>> boot_y;
    std::vector<DecisionTree<T>> trees;

    Matrix<T> dummy_X;
    Matrix<T> dummy_y;

    Matrix<T> encode_forest() const;

public:
    RandomForest(const Matrix<T>* X, const Matrix<T>* y, size_t n_trees, size_t max_depth);

    void fit() override;
    Matrix<T> predict(const Matrix<T>& X) const override;
    LearningModelType get_type() const override;
    std::unique_ptr<LearningModel<T>> clone() const override;

    const Matrix<T>& get_weights() const override;
    void set_weights(const Matrix<T>& m) override;
};

template <class T>
Matrix<T> RandomForest<T>::encode_forest() const {
    std::vector<std::vector<float>> rows;

    rows.push_back({(float)n_trees, 0.f, 0.f, 0.f, 0.f, 0.f});

    for (size_t t = 0; t < trees.size(); t++) {
        const Matrix<T>& tree_weights = trees[t].get_weights();
        size_t node_count = tree_weights.get_rows_count();

        rows.push_back({(float)node_count, 0.f, 0.f, 0.f, 0.f, 0.f});

        for (size_t i = 0; i < node_count; i++) {
            std::vector<float> row(6);

            for (size_t j = 0; j < 6; j++) {
                row[j] = (float)tree_weights[i][j];
            }

            rows.push_back(row);
        }
    }

    Matrix<T> result;
    result.resize_dims(rows.size(), 6);
    for (size_t i = 0; i < rows.size(); i++) {
        for (size_t j = 0; j < 6; j++) {
            result[i][j] = (T)rows[i][j];
        }
    }

    return result;
}

template <class T>
RandomForest<T>::RandomForest(const Matrix<T>* X, const Matrix<T>* y, size_t n_trees, size_t max_depth): 
    SupervisedLearningModel<T>(X, y), n_trees(n_trees), max_depth(max_depth) {
    dummy_X.resize_dims(1, 1); dummy_y.resize_dims(1, 1);
}

template <class T>
void RandomForest<T>::fit() {
    const Matrix<T>& X = *this->get_X();
    const Matrix<T>& y = *this->get_y();

    size_t n = X.get_rows_count();
    size_t n_features = X.get_cols_count();

    boot_X.clear();
    boot_y.clear();
    trees.clear();
    boot_X.resize(n_trees);
    boot_y.resize(n_trees);

    std::mt19937 rng(12345);
    std::uniform_int_distribution<size_t> pick(0, n - 1);

    for (size_t t = 0; t < n_trees; t++) {
        boot_X[t].resize_dims(n, n_features);
        boot_y[t].resize_dims(n, 1);

        for (size_t s = 0; s < n; s++) {
            size_t row = pick(rng);

            for (size_t j = 0; j < n_features; j++) {
                boot_X[t][s][j] = X[row][j];
            }

            boot_y[t][s][0] = y[row][0];
        }

        TreeEncoder encoder;
        TreeDecoder decoder;
        DecisionTree<T> dt(&boot_X[t], &boot_y[t], max_depth, &encoder, &decoder);
        dt.fit();
        trees.push_back(std::move(dt));
    }

    this->weights = encode_forest();
}

template <class T>
Matrix<T> RandomForest<T>::predict(const Matrix<T>& X) const {
    if (trees.empty()) {
        throw std::runtime_error("RandomForest: model not fitted");
    }

    size_t n = X.get_rows_count();
    Matrix<T> result;
    result.resize_dims(n, 1);

    for (size_t i = 0; i < n; i++) {
        std::map<float, size_t> votes;

        for (const DecisionTree<T>& tree : trees) {
            Matrix<T> row_X;
            row_X.resize_dims(1, X.get_cols_count());

            for (size_t j = 0; j < X.get_cols_count(); j++) {
                row_X[0][j] = X[i][j];
            }

            Matrix<T> pred = tree.predict(row_X);
            votes[(float)pred[0][0]]++;
        }

        float best_label = 0.f;
        size_t best_count = 0;
        for (const auto& vote : votes) {
            if (vote.second > best_count) {
                best_count = vote.second;
                best_label = vote.first;
            }
        }

        result[i][0] = (T)best_label;
    }

    return result;
}

template <class T>
LearningModelType RandomForest<T>::get_type() const {
    return LearningModelType::RandomForest;
}

template <class T>
std::unique_ptr<LearningModel<T>> RandomForest<T>::clone() const {
    return std::make_unique<RandomForest<T>>(*this);
}

template <class T>
const Matrix<T>& RandomForest<T>::get_weights() const {
    return this->weights;
}

template <class T>
void RandomForest<T>::set_weights(const Matrix<T>& m) {
    this->weights = m;
    trees.clear();
    boot_X.clear();
    boot_y.clear();

    if (m.get_rows_count() == 0) return;

    size_t loaded_n_trees = (size_t)m[0][0];
    size_t cursor = 1;

    for (size_t t = 0; t < loaded_n_trees; t++) {
        if (cursor >= m.get_rows_count()) {
            throw std::runtime_error("RandomForest decode: unexpected end of weights matrix");
        }
        size_t node_count = (size_t)m[cursor][0];
        cursor += 1;

        Matrix<T> block;
        block.resize_dims(node_count, 6);

        for (size_t i = 0; i < node_count; i++) {
            for (size_t j = 0; j < 6; j++) {
                block[i][j] = m[cursor + i][j];
            }
        }

        cursor += node_count;
        TreeEncoder decoder_enc;
        TreeDecoder decoder_dec;
        DecisionTree<T> dt(&dummy_X, &dummy_y, 1, &decoder_enc, &decoder_dec);
        dt.set_weights(block);
        trees.push_back(std::move(dt));
    }

    n_trees = trees.size();
}
