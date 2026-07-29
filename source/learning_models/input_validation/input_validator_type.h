#pragma once

enum class InputValidatorType {
    // Regression
    RANSACValidator = (1 << 0),
    LinearRegressionValidator = (1 << 1),
    PolynomialRegressionValidator = (1 << 2),

    // Classification
    LogisticRegressionValidator = (1 << 3),
    DecisionTreeValidator = (1 << 4),
    RandomForestValidator = (1 << 5),

    // Clustering
    KMeansValidator = (1 << 6),
    DBSCANValidator = (1 << 7),
    HierarchicalClusteringValidator = (1 << 8)
};