#ifndef VECN_H
#define VECN_H

#include <iostream>
#include <cmath>

using namespace std;

// *** vecN class ***

class vecN {
protected:
    double* data;
    int dim; // Dimension
public:
    // Constructor and Destructor
    vecN() : data(nullptr), dim(0) {}
    vecN(int size) : dim(size){
        data = new double[size]();
    }
    ~vecN() { delete[] data; }

    // Operator Overloading
    inline double operator[](int index) { return data[index]; }
    inline double& operator[](int index) const { return data[index]; }
    inline vecN& operator=(const vecN& other){
        if (this != &other) {
            delete[] data; // Free existing memory
            dim = other.dim;
            data = new double[dim];
            for (int i = 0; i < dim; i++) {
                data[i] = other.data[i]; // Copy data
            }
        }
        return *this;
    };

    // Methods
    inline void set(int index, double value) {
        data[index] = value;
    }
    inline double getDim() const {
        return dim;
    }
    double norm() const {
        float sum = 0.0f;
        for (int i = 0; i < dim; i++) {
            sum += data[i] * data[i];
        }
        return sqrt(sum);
    }
};

// Methods
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

// Operator overloading
inline ostream& operator<<(ostream& out, const vecN& v){
    out << "Dims: " << v.getDim() << "d, Data: [";
    for (int i = 0; i < v.getDim(); i++) {
        out << v[i] << (i < v.getDim() - 1 ? ", " : "");
    }
    out << "]";
    return out;
}



// *** vec3 class ***

enum class CoordType {
    UNDEFINED,
    CARTESIAN, // Cartesian coordinates
    SPHERICAL, // Spherical coordinates
};

class vec3 : public vecN {
private:
    CoordType cType = CoordType::UNDEFINED;
public:

    // Constructor overloading
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f,
         CoordType type=CoordType::UNDEFINED){
        data = new double[3]{x, y, z};
        dim = 3;
        cType = type;
    }

    inline CoordType getCType() const {
        return cType;
    }

    // Convert the vector's coordinates to Spherical coordinates
    void convToSpherical() {
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

    // Convert the vector's coordinates to Cartesian coordinates
    void convToCartesian() {
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

    inline vec3& operator=(const vec3& other){
        if (this != &other) {
            data[0] = other[0];
            data[1] = other[1];
            data[2] = other[2];
            cType = other.getCType();
        }
        return *this;
    };
};

// Operator Overloading
// They are inline functions, since we don't want the new vec3 object destroyed
// as the call stack unwinds.
inline ostream& operator<<(ostream& out, const vec3& v){
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

inline vec3 operator-(const vec3& a) {
    return vec3(-a[0], -a[1], -a[2], a.getCType());
}
inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
inline vec3 operator*(const vec3& v, const double c) {
    return vec3(v[0] * c, v[1] * c, v[2] * c);
}
inline vec3 operator*(const double c, const vec3& v) {
    return vec3(v[0] * c, v[1] * c, v[2] * c);
}

// Methods
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

#endif // VECN_H
