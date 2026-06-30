# Machine-Learning-Algorithms

A collection of classic machine learning algorithms implemented from scratch in C++ with no external dependencies.

## Structure

- `supervised_learning/regression/` — linear regression, polynomial regression, RANSAC
- `supervised_learning/classification/` — logistic regression, decision tree, random forest, SVM, KNN, Naive Bayes, gradient boosting
- `unsupervised_learning/clustering/` — k-means, DBSCAN, hierarchical clustering
- `loss_functions/` — mean squared error, mean absolute error, Huber loss, binary cross entropy, categorical cross entropy (each with base, L1, and L2 regularized variants)
- `encoding/` — label, one-hot, ordinal, frequency encoding
- `scaling/` — min-max, standard (Z-score), robust, max-abs scalers

## Style

Each algorithm is a single self-contained `.cpp` file: global training data (`X`, `Y`), global model parameters, a prediction function, and a `main` that trains the model. Gradient-based models share a numerical `calculate_derivative_at` + `gradient_descent` pair; tree-based models share a `Node` struct with index-based children; distance-based models share a `squared_distance` helper. Loss functions take `(input, labels, function)` (plus `weights` for the regularized variants) and expose a `regularization_lambda` global for the L1/L2 penalty strength. Encoders and scalers expose a `*_fit` function that learns per-feature state from the dataset and a single-sample transform function that reuses it.

## Build

```sh
g++ -std=c++17 -Wall -Wextra path/to/algorithm.cpp -o algorithm
```
