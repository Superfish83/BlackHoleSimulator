#include <stdexcept>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "vectors.h"

using namespace vectors;

inline vecN& vecN::operator=(const vecN& other) {
    if (this != &other) { 
        delete[] data; // Free existing memory
        dim = other.dim;
        data = new double[dim];
        for (int i = 0; i < dim; i++) {
            data[i] = other.data[i]; // Copy data
            std::cout << other.data[i] << " ";
        }
    }
    return *this;
}
double vecN::norm() const {
    float sum = 0.0f;
    for (int i = 0; i < dim; i++) {
        sum += data[i] * data[i];
    }
    return std::sqrt(sum);
}
double vectors::dot(const vecN& a, const vecN& b) {
        if (a.getDim() != b.getDim()) {
            throw std::invalid_argument("Vectors must be of the same size for dot product.");
        }
        double result = 0.0;
        for (int i = 0; i < a.getDim(); i++) {
            result += a[i] * b[i];
        }
        return result;
    }