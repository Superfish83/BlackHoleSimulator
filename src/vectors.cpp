#include <stdexcept>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "vectors.h"

using namespace vectors;

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

inline vec4 vec3::getVec4() const {
    return vec4(0.0f, data[0], data[1], data[2], cType);
}
inline vec3 vec4::getVec3() const {
    return vec3(data[1], data[2], data[3], cType);
}

void vec3::convToSpherical() {
    if(cType != CoordType::CARTESIAN) {
        throw std::invalid_argument("The vector is not in Cartesian coordinates.");
    }

    cType = CoordType::SPHERICAL;
    // Convert from Cartesian to spherical
    double x = data[0];
    double y = data[1];
    double z = data[2];

    data[0] = std::sqrt(x*x + y*y + z*z); // r
    data[1] = std::atan2(std::sqrt(x*x + y*y), z); // theta
    data[2] = std::atan2(y, x); // phi
}
void vec3::convToCartesian() {
    if(cType != CoordType::SPHERICAL) {
        throw std::invalid_argument("The vector is not in Spherical coordinates.");
    }

    cType = CoordType::CARTESIAN;
    // Convert from spherical to Cartesian
    double r = data[0];
    double theta = data[1];
    double phi = data[2];
    
    data[0] = r * std::sin(theta) * std::cos(phi);
    data[1] = r * std::sin(theta) * std::sin(phi);
    data[2] = r * std::cos(theta);
}

void vec4::convToSpherical() {
    if(cType != CoordType::CARTESIAN) {
        throw std::invalid_argument("The vector is not in Cartesian coordinates.");
    }

    cType = CoordType::SPHERICAL;
    // Convert from Cartesian to spherical
    double x = data[1];
    double y = data[2];
    double z = data[3];

    data[1] = std::sqrt(x*x + y*y + z*z); // r
    data[2] = std::atan2(std::sqrt(x*x + y*y), z); // theta
    data[3] = std::atan2(y, x); // phi
}
void vec4::convToCartesian() {
    if(cType != CoordType::SPHERICAL) {
        throw std::invalid_argument("The vector is not in Spherical coordinates.");
    }

    cType = CoordType::CARTESIAN;
    // Convert from spherical to Cartesian
    double r = data[1];
    double theta = data[2];
    double phi = data[3];
    
    data[1] = r * std::sin(theta) * std::cos(phi);
    data[2] = r * std::sin(theta) * std::sin(phi);
    data[3] = r * std::cos(theta);
}