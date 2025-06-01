#include <stdexcept>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "rays.h"

using namespace std;

double vecN::norm() const {
    float sum = 0.0f;
    for (int i = 0; i < dim; i++) {
        sum += data[i] * data[i];
    }
    return sqrt(sum);
}
double dot(const vecN& a, const vecN& b) {
    if (a.getDim() != b.getDim()) {
        throw invalid_argument("Vectors must be of the same size for dot product.");
    }
    double result = 0.0;
    for (int i = 0; i < a.getDim(); i++) {
        result += a[i] * b[i];
    }
    return result;
}
double distance(const vec3& a_, const vec3& b_){
    vec3 a, b;
    a = a_;
    b = b_;


    auto cta = a.getCType();
    auto ctb = b.getCType();

    if(cta == CoordType::UNDEFINED || ctb == CoordType::UNDEFINED){
        throw invalid_argument("Can't compute distance for non-geometric vectors");
    }

    if(cta == CoordType::CARTESIAN){
        a.set(0, 1);
        a.convToSpherical();
    }
    if(ctb == CoordType::CARTESIAN){
        b.set(0, 1);
        b.convToSpherical();
    }
    return (a-b).norm();
}

inline ostream& operator<<(ostream& out, const vecN& v){
    CoordType cType = v.getCType();
    out << "Dims: " << v.getDim() << "d, Data: [";
    for (int i = 0; i < v.getDim(); i++) {
        out << v[i] << (i < v.getDim() - 1 ? ", " : "");
    }
    out << "]";
    if(cType != CoordType::UNDEFINED) {
        switch(v.getCType()) {
            case CoordType::CARTESIAN: out << " (Cartesian)"; break;
            case CoordType::SPHERICAL: out << " (Spherical)"; break;
        }
    }
    return out;
}

void vec3::convToSpherical() {
    if(cType != CoordType::CARTESIAN) {
        throw invalid_argument("The vector is not in Cartesian coordinates.");
    }

    cType = CoordType::SPHERICAL;
    // Convert from Cartesian to spherical
    double x = data[0];
    double y = data[1];
    double z = data[2];

    data[0] = sqrt(x*x + y*y + z*z); // r
    data[1] = atan2(sqrt(x*x + y*y), z); // theta
    data[2] = atan2(y, x); // phi
}
void vec3::convToCartesian() {
    if(cType != CoordType::SPHERICAL) {
        throw invalid_argument("The vector is not in Spherical coordinates.");
    }

    cType = CoordType::CARTESIAN;
    // Convert from spherical to Cartesian
    double r = data[0];
    double theta = data[1];
    double phi = data[2];
    
    data[0] = r * sin(theta) * cos(phi);
    data[1] = r * sin(theta) * sin(phi);
    data[2] = r * cos(theta);
}
