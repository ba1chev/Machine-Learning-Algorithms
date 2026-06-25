# Machine-Learning-Algorithms

A collection of classic machine learning algorithms implemented from scratch in C++ with no external dependencies.

## Structure

- `supervised_learning/regression/` — linear regression, polynomial regression, RANSAC
- `supervised_learning/classification/` — logistic regression, decision tree, random forest, SVM, KNN
- `unsupervised_learning/clustering/` — k-means, DBSCAN, hierarchical clustering

## Style

Each algorithm is a single self-contained `.cpp` file: global training data (`X`, `Y`), global model parameters, a prediction function, and a `main` that trains the model. Gradient-based models share a numerical `calculate_derivative_at` + `gradient_descent` pair; tree-based models share a `Node` struct with index-based children; distance-based models share a `squared_distance` helper.

## Build

```sh
g++ -std=c++17 -Wall -Wextra path/to/algorithm.cpp -o algorithm
```
