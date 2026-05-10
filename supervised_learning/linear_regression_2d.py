import numpy as np
import matplotlib.pyplot as plt
from typing import Callable, List


def plot_function(function: Callable, x_coordinates: List[float], color: str, input_label: str) -> None:
    y_coordinates = [function(current_x) for current_x in x_coordinates]
    plt.plot(x_coordinates, y_coordinates, c = color, label = input_label)


def scatter_function(function: Callable, x_coordinates: List[float], color: str, input_label: str) -> None:
    y_coordinates = [function(current_x) for current_x in x_coordinates]
    plt.scatter(x_coordinates, y_coordinates, c = color, label = input_label)


def scatter_function_points(x_coordinates: List[float], y_coordinates: List[float], color: str, input_label: str) -> None:
    plt.scatter(x_coordinates, y_coordinates, c = color, label = input_label)


def generate_random_x_coordinates(left_bound: float, right_bound: float, count_of_points: int) -> List[float]:
    x_random_coordinates = [np.random.uniform(left_bound, right_bound) for _ in range(count_of_points)]
    return x_random_coordinates


def add_noise_to_y_coordinates(function: Callable, x_coordinates: List[float], mu: float, omega: float) -> List[float]:
    y_noise_coordinates = [function(current_x) + np.random.normal(loc = mu, scale = omega) for current_x in x_coordinates]
    return y_noise_coordinates


def get_trained_2_params_model(x_coordinates: List[float], y_coordinates: List[float], model_training_lifecycle: int) -> Callable:
    first_parameter = np.random.uniform(0, 1)
    second_parameter = np.random.uniform(0, 1)

    result_model = None
    learing_rate = 0.01

    for _ in range(model_training_lifecycle):
        current_model = lambda x: first_parameter * x + second_parameter
        y_coordinates_prediction = [current_model(current_x) for current_x in x_coordinates]

        mean_squered_error = 0  
        for current_actual_y, current_prediction_y in zip(y_coordinates_prediction, y_coordinates):
            mean_squered_error += abs(current_actual_y - current_prediction_y)
        mean_squered_error /= len(y_coordinates)

        first_parameter_gradient = 0
        second_parameter_gradient = 0

        for current_x, current_actual_y, current_prediction_y in zip(x_coordinates, y_coordinates, y_coordinates_prediction):
            current_error = current_prediction_y - current_actual_y
            first_parameter_gradient += 2 * current_error * current_x
            second_parameter_gradient += 2 * current_error

        first_parameter_gradient /= len(y_coordinates)
        second_parameter_gradient /= len(y_coordinates)

        first_parameter += -first_parameter_gradient * learing_rate
        second_parameter += -second_parameter_gradient * learing_rate
        result_model = current_model

    return result_model


if __name__ == "__main__":
    real_function = lambda x: 5 * x + 2
    x_coordinates = generate_random_x_coordinates(0, 10, 100)
    y_noise_coordinates = add_noise_to_y_coordinates(real_function, x_coordinates, 0, 5)
    trained_model = get_trained_2_params_model(x_coordinates, y_noise_coordinates, 100)

    scatter_function_points(x_coordinates, y_noise_coordinates, "g", "Noisy data")
    plot_function(real_function, x_coordinates, "r", "Real function")
    plot_function(trained_model, x_coordinates, "b", "Trained model function")
    plt.legend()
    plt.show()


