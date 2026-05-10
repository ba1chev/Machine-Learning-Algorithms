#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

float generateRandomPoint(int64_t leftBound = 0, int64_t rightBound = 1) {
    if (rightBound <= leftBound) {
        throw std::runtime_error("Right bound must be greater than left bound");
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<float> distrib(leftBound, rightBound);

    return distrib(generator);
}

std::vector<float> generateRandomVector(size_t countOfXCoordinates, int64_t leftBound = 0, int64_t rightBound = 1) {
    if (!countOfXCoordinates) {
        throw std::runtime_error("Count of x coordiantes must be greater than zero");
    }

    std::vector<float> result;
    result.reserve(countOfXCoordinates);

    for (size_t i = 0; i < countOfXCoordinates; i++) {
        result.push_back(generateRandomPoint(leftBound, rightBound));
    }
    
    return result;
}

void saveDataToFile(const std::string& fileName, const std::vector<float>& data) {
    if (!fileName.length()) {
        throw std::runtime_error("Input string must be with size greater than zero");
    }

    std::ofstream file(fileName.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("File is not opened");
    }

    for (size_t i = 0; i < data.size(); i++) {
        if (i != data.size() - 1) {
            file << data[i] << ",";
        } else {
            file << data[i] << std::endl;
        }
    }
    
    file.close();
}

int main() {
    saveDataToFile("result.csv", generateRandomVector(10, -5, 5));
    
    return 0;
}