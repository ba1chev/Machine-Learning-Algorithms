#pragma once

enum class LearningModelType {
    // Regression
    RANSAC = (1 << 0),
    LinearRegression = (1 << 1),
    PolynomialRegression = (1 << 2),

    // Classification
    LogisticRegression = (1 << 3),
    DecisionTree = (1 << 4),
    RandomForest = (1 << 5),

    // Clustering
    KMeans = (1 << 6),
    DBSCAN = (1 << 7),
    HierarchicalClustering = (1 << 8)
};