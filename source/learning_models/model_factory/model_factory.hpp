#pragma once

#include <memory>
#include <vector>
#include <cstddef>
#include <stdexcept>

#include "source/learning_models/model_factory/abstract_model_factory.hpp"
#include "source/learning_models/encoding/encoder_factory/encoder_factory.hpp"
#include "source/learning_models/decoding/decoder_factory/decoder_factory.hpp"
#include "source/learning_models/supervised_learning/regression/ransac/ransac.hpp"
#include "source/learning_models/unsupervised_learning/clustering/kmeans/kmeans.hpp"
#include "source/learning_models/unsupervised_learning/clustering/dbscan/dbscan.hpp"
#include "source/learning_models/supervised_learning/classification/decision_tree/decision_tree.hpp"
#include "source/learning_models/supervised_learning/classification/random_forest/random_forest.hpp"
#include "source/learning_models/supervised_learning/regression/linear_regression/linear_regression.hpp"
#include "source/learning_models/supervised_learning/classification/logistic_regression/logistic_regression.hpp"
#include "source/learning_models/supervised_learning/regression/polynomial_regression/polynomial_regression.hpp"
#include "source/learning_models/unsupervised_learning/clustering/hierarchical_clustering/hierarchical_clustering.hpp"

template <class T>
class ModelFactory: public AbstractModelFactory<T> {
public:
    std::unique_ptr<LearningModel<T>> create(LearningModelType model_type, const Matrix<T>* X, const Matrix<T>* y,
        const std::vector<double>& hyperparameters) const override;
};

template <class T>
std::unique_ptr<LearningModel<T>> ModelFactory<T>::create(LearningModelType model_type, const Matrix<T>* X, const Matrix<T>* y,
    const std::vector<double>& hyperparameters) const {
    switch (model_type) {
        case LearningModelType::LinearRegression: {
            return std::make_unique<LinearRegression<T>>(X, y);
        }
        case LearningModelType::LogisticRegression: {
            return std::make_unique<LogisticRegression<T>>(X, y);
        }
        case LearningModelType::PolynomialRegression: {
            if (hyperparameters.size() < 1) {
                throw std::runtime_error("PolynomialRegression requires a degree hyperparameter");
            }
            size_t degree = (size_t)hyperparameters[0];
            return std::make_unique<PolynomialRegression<T>>(X, y, degree);
        }
        case LearningModelType::RANSAC: {
            if (hyperparameters.size() < 3) {
                throw std::runtime_error("RANSAC requires iterations, sample_size and inlier_threshold hyperparameters");
            }

            size_t iterations = (size_t)hyperparameters[0];
            size_t sample_size = (size_t)hyperparameters[1];
            T inlier_threshold = (T)hyperparameters[2];
            return std::make_unique<RANSAC<T>>(X, y, iterations, sample_size, inlier_threshold);
        }
        case LearningModelType::DecisionTree: {
            if (hyperparameters.size() < 1) {
                throw std::runtime_error("DecisionTree requires a max_depth hyperparameter");
            }

            size_t max_depth = (size_t)hyperparameters[0];
            std::shared_ptr<const EncoderTemplate<Tree<DecisionNode>, Matrix<float>>> encoder =
                EncoderFactory<Tree<DecisionNode>, Matrix<float>>().create(EncoderType::TREE_DATA_ENCODER);
            std::shared_ptr<const DecoderTemplate<Matrix<float>, Tree<DecisionNode>>> decoder =
                DecoderFactory<Matrix<float>, Tree<DecisionNode>>().create(DecoderType::TREE_DATA_DECODER);
            return std::make_unique<DecisionTree<T>>(X, y, max_depth, encoder, decoder);
        }
        case LearningModelType::RandomForest: {
            if (hyperparameters.size() < 2) {
                throw std::runtime_error("RandomForest requires n_trees and max_depth hyperparameters");
            }

            size_t n_trees = (size_t)hyperparameters[0];
            size_t max_depth = (size_t)hyperparameters[1];
            return std::make_unique<RandomForest<T>>(X, y, n_trees, max_depth);
        }
        case LearningModelType::KMeans: {
            if (hyperparameters.size() < 1) {
                throw std::runtime_error("KMeans requires a k hyperparameter");
            }

            size_t k = (size_t)hyperparameters[0];
            size_t max_iters = hyperparameters.size() >= 2 ? (size_t)hyperparameters[1] : 100;
            return std::make_unique<KMeans<T>>(X, k, max_iters);
        }
        case LearningModelType::DBSCAN: {
            if (hyperparameters.size() < 2) {
                throw std::runtime_error("DBSCAN requires eps and min_samples hyperparameters");
            }

            double eps = hyperparameters[0];
            size_t min_samples = (size_t)hyperparameters[1];
            return std::make_unique<DBSCAN<T>>(X, eps, min_samples);
        }
        case LearningModelType::HierarchicalClustering: {
            if (hyperparameters.size() < 1) {
                throw std::runtime_error("HierarchicalClustering requires a k hyperparameter");
            }

            size_t k = (size_t)hyperparameters[0];
            return std::make_unique<HierarchicalClustering<T>>(X, k);
        }
        default: {
            throw std::runtime_error("Unsupported learning model type");
        }
    }
}
