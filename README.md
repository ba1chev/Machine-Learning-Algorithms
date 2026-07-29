# Machine Learning Algorithms

A C++17 computational engine for training supervised and unsupervised ML models and making predictions on numerical CSV data.

## Purpose

This project is a **pure computational engine** — its only responsibility is to take clean numerical input, train a model, and produce weights or predictions. Data preprocessing, feature engineering, and result interpretation are the consumer's responsibility.

### Use Cases

**LLM agent tools (e.g. AI alert monitoring agents)**
An agent that needs historical context would typically receive thousands of raw data rows as input — expensive in tokens, slow to process, and prone to hallucination. Instead, this engine trains a model on that historical data and produces a compact set of weights and predictions. The agent receives a handful of rows describing how metrics correlated over time, not the raw history. The consumer (agent orchestrator) decides how to format and present those results; the engine only computes them.

**MCP tool for Claude and other AI assistants**
The engine can be exposed as an MCP (Model Context Protocol) tool, allowing Claude or any MCP-compatible assistant to call `train` and `predict` directly as functions during a conversation. This gives the assistant an on-demand computational backend for historical data analysis without any middleware.

**Offline correlation analysis**
Given a preprocessed numerical dataset, train a model to capture the relationship between variables, persist the weights, and later run predictions on new data points — without re-training. The engine handles the math; everything else is up to the caller.

## Features

- **Multiple model families** — regression (linear, polynomial, RANSAC), classification (logistic regression, decision tree, random forest), and clustering (k-means, DBSCAN, hierarchical)
- **Automatic feature scaling** — optional z-score or min-max normalization, fit on training data and persisted with the model so predictions apply the same transform
- **Training metrics** — quality metrics computed on the training data and printed after each `train`, so the caller can judge how much to trust the model
- **Model persistence** — trained weights, degree, and scaler parameters are serialized to a portable text file for later reuse
- **Selectable optimizers** — gradient descent, BFGS, or L-BFGS for regression and classification models, chosen from the command line
- **Zero runtime dependencies** — pure C++17 standard library, builds to a single executable

## How It Works

```
numerical .csv  →  train  →  model file (weights)
model file + numerical .csv  →  predict  →  predictions .csv
```

## Usage

```bash
# Train
./build/main train <model> <data.csv> [hyperparams] [optimizer] [scaler] <model_out>

# Predict
./build/main predict <model_file> <input.csv> <output.csv>
```

### Supported Models

| Model | Command | Hyperparameters |
|-------|---------|-----------------|
| Linear Regression | `linear_regression` | — |
| Polynomial Regression | `polynomial_regression` | `degree` |
| RANSAC | `ransac` | `iterations sample_size inlier_threshold` |
| Logistic Regression | `logistic_regression` | — |
| Decision Tree | `decision_tree` | `max_depth` |
| Random Forest | `random_forest` | `n_trees max_depth` |
| K-Means | `k_means` | `k [max_iters]` |
| DBSCAN | `dbscan` | `eps min_samples` |
| Hierarchical Clustering | `hierarchical_clustering` | `k` |

### Optimizers (optional)

`gradient_descent` (default) · `bfgs` · `lbfgs`

Only regression (`linear_regression`, `polynomial_regression`, `ransac`) and classification (`logistic_regression`) models train through an optimizer. Requesting a non-default optimizer for any other model (decision tree, random forest, or a clustering model) is an error. The `optimizer` and `scaler` tokens are both optional and recognized by value, so their order is interchangeable.

### Scalers (optional)

`none` · `normal` (z-score) · `min_max` ([0, 1])

### Metrics

After training, the engine prints quality metrics computed on the training data. The set is chosen automatically based on the model family:

| Family | Metrics |
|--------|---------|
| Regression | MSE, RMSE, MAE, R² |
| Classification | Accuracy, Precision, Recall, F1 |
| Clustering | none — prints the number of clusters found (`Clusters: N`) |

### CSV Format

- Comma-separated, numerical values only
- **Regression / classification** — the last column is the target variable. The same layout applies to the `predict` input CSV: it must still include the target column (it is read but ignored during prediction).
- **Clustering** — there is no target column; every column is a feature (the data is unlabeled).

## Interpreting the Results

### The model file (`<model_out>`)

`train` writes a portable text file describing everything needed to reproduce the model. The layout is fixed:

```
<model type>              # e.g. linear_regression
<degree>                  # polynomial degree, 0 for non-polynomial models
<scaler type>             # none | normal | min_max
<param_rows> <param_cols> # dimensions of the scaler parameter block (0 0 if none)
<scaler parameters...>    # one value per line, present only when a scaler was used
<rows> <cols>             # dimensions of the weight matrix
<weights...>              # one value per line, row-major
```

Example (`linear_regression`, no scaler, `y = 2·x1 + 3·x2 + 1`):

```
linear_regression   ← model type
0                   ← degree (unused here)
none                ← no scaler
0 0                 ← no scaler parameters
3 1                 ← weight matrix is 3×1
2.38289             ← weight for x1  (target: 2)
2.35201             ← weight for x2  (target: 3)
0.624393            ← bias / intercept (target: 1)
```

When a scaler is used, the parameter block precedes the weights. For `normal` each feature contributes a mean and a standard deviation; for `min_max`, a min and a max. `predict` reads these and applies the identical transform to new inputs, so you never re-supply scaler settings at predict time.

- **Regression models** — weights are the fitted coefficients (last row is the bias when the model uses one). Treat them as the learned linear/polynomial relationship.
- **Classification models** — logistic regression stores coefficients; tree/forest models store a serialized tree structure rather than coefficients, so those files are not meant to be read by hand.
- **Clustering models** — the weight matrix holds the learned cluster centroids (one row per cluster, one column per feature). `predict` assigns each new point to the nearest centroid.

### The predictions file (`<output.csv>`)

`predict` writes a single-column CSV, one predicted value per input row, in the same order as the input:

```
7.7113
14.8291
14.8909
29.0647
26.7744
```

- **Regression** — each value is the predicted target (a real number).
- **Classification** — each value is the predicted class label (`0` / `1`). Round if you see floating-point noise.
- **Clustering** — each value is the assigned cluster id (`0`, `1`, `2`, …), one per input row.

### The training metrics (stdout)

`train` prints metrics computed on the training data. They describe fit quality on the data the model has already seen — use them to decide how much to trust the weights, not as a substitute for a held-out evaluation.

## Build

```bash
cd source && ./build.sh
```

Requires: `g++` with C++17 support.
